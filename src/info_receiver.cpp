#include "info_receiver.h"
#include "cancellable_streambuf.h"
#include "inlet_connection.h"
#include "stream_info_impl.h"
#include <chrono>
#include <exception>
#include <istream>
#include <loguru.hpp>
#include <memory>
#include <sstream>
#include <string>
#include <pugixml.hpp>

lsl::info_receiver::info_receiver(inlet_connection &conn) : conn_(conn) {
	conn_.register_onlost(this, &fullinfo_upd_);
}

lsl::info_receiver::~info_receiver() {
	try {
		conn_.unregister_onlost(this);
		if (info_thread_.joinable()) info_thread_.join();
	} catch (std::exception &e) {
		LOG_F(ERROR, "Unexpected error during destruction of an info_receiver: %s", e.what());
	} catch (...) { LOG_F(ERROR, "Severe error during info receiver shutdown."); }
}

const lsl::stream_info_impl &lsl::info_receiver::info(double timeout) {
	std::unique_lock<std::mutex> lock(fullinfo_mut_);
	auto info_ready = [this]() { return fullinfo_ || conn_.lost() || conn_.shutdown(); };
	if (!info_ready()) {
		fullinfo_ = 0;
		// start thread if not yet running
		if (!info_thread_.joinable()) info_thread_ = std::thread(&info_receiver::info_thread, this);
		// wait until we are ready to return a result (or we time out)
		if (timeout >= FOREVER)
			fullinfo_upd_.wait(lock, info_ready);
		else if (!fullinfo_upd_.wait_for(lock, std::chrono::duration<double>(timeout), info_ready))
			throw timeout_error("The info() operation timed out.");
	}
	if (conn_.lost())
		throw lost_error("The stream read by this inlet has been lost. To recover, you need to "
						 "re-resolve the source and re-create the inlet.");

	return *fullinfo_;
}

void lsl::info_receiver::info_thread() {
	conn_.acquire_watchdog();
	loguru::set_thread_name(("I_" + conn_.type_info().name().substr(0, 10) + "_" + conn_.type_info().type().substr(0, 3)).c_str());
	try {
		while (!conn_.lost() && !conn_.shutdown()) {
			try {
				std::unique_lock<std::mutex> command_lock(commands_mut_);
				bool has_command = !commands_.empty();
				command_lock.unlock();
				// make a new stream buffer & stream
				cancellable_streambuf buffer;
				buffer.register_at(&conn_);
				std::iostream server_stream(&buffer);
				// connect...
				if (buffer.connect(conn_.get_tcp_endpoint()) == nullptr) {
					break;
				}
				if (has_command) {
					// send the command
					server_stream << "LSL:command\r\n" << commands_ << "\r\n" << std::flush;
				} else {
					// send the query
					server_stream << "LSL:fullinfo\r\n" << std::flush;
				}
				// receive and parse the response
				std::ostringstream os;
				os << server_stream.rdbuf();
				stream_info_impl info;
				std::string msg = os.str();
				if (msg.empty()) continue;
				info.from_fullinfo_message(msg);
				// if this is not a valid streaminfo we retry
				if (!info.created_at()) continue;
				// store the result for pickup & return
				{
					std::lock_guard<std::mutex> lock(fullinfo_mut_);
					fullinfo_ = std::make_shared<stream_info_impl>(info);
				}
				if (has_command) {
					commands_.clear();
				}
				fullinfo_upd_.notify_all();
				conn_.update_receive_time(lsl_clock());
				command_lock.lock();
				commands_upd_.wait_for(command_lock, std::chrono::milliseconds(100), [&] { return !commands_.empty(); });
				continue;
			} catch (err_t) {
				// connection-level error: closed, reset, refused, etc.
				conn_.try_recover_from_error();
			} catch (std::exception &e) {
				// parsing-level error: intermittent disconnect or invalid protocol
				LOG_F(ERROR, "Error while receiving the stream info (%s); retrying...", e.what());
				conn_.try_recover_from_error();
			}
		}
	} catch (lost_error &) {}
	conn_.release_watchdog();
}

const lsl::stream_info_impl &lsl::info_receiver::send_commands(std::string commands, double timeout) {
	{
		std::lock_guard<std::mutex> lock(commands_mut_);
		commands_ = commands;
	}
	std::unique_lock<std::mutex> lock(fullinfo_mut_);
	auto info_ready = [this]() { return (fullinfo_ && commands_.empty()) || conn_.lost() || conn_.shutdown(); };
	if (!info_ready()) {
		fullinfo_ = 0;
		// start thread if not yet running
		if (!info_thread_.joinable()) info_thread_ = std::thread(&info_receiver::info_thread, this);
		// we have command to process, wake info_thread from wait_for()
		commands_upd_.notify_all();
		// wait until we are ready to return a result (or we time out)
		if (timeout >= FOREVER)
			fullinfo_upd_.wait(lock, info_ready);
		else if (fullinfo_upd_.wait_for(lock, std::chrono::duration<double>(timeout), info_ready))
			throw timeout_error("The info() operation timed out.");
	}
	if (conn_.lost())
		throw lost_error("The stream read by this inlet has been lost. To recover, you need to "
						 "re-resolve the source and re-create the inlet.");
	
	return *fullinfo_;
}

std::string lsl::info_receiver::make_command(const std::string &command, const std::string &xpath, 
    const std::string &name, const std::string &value, const std::string &text) {
    
    pugi::xml_document doc;
    pugi::xml_node cmd = doc.append_child();
	
	cmd.set_name(command.c_str());

    if (!xpath.empty())
        cmd.append_attribute("xpath").set_value(xpath.c_str());
    if (!name.empty())
        cmd.append_attribute("name").set_value(name.c_str());
    if (!value.empty())
        cmd.append_attribute("value").set_value(value.c_str());
    if (!text.empty())
        cmd.append_attribute("text").set_value(text.c_str());

	// write the doc to a stream
	std::ostringstream os;
	doc.print(os, "\t", pugi::format_no_escapes | pugi::format_raw | pugi::format_attribute_single_quote | pugi::format_no_declaration);
	// and get the string
	return std::move(os.str());
}
