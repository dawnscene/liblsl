#include "command_sender.h"
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

lsl::command_sender::command_sender(inlet_connection &conn) : conn_(conn) {
	conn_.register_onlost(this, &fullinfo_upd_);
}

lsl::command_sender::~command_sender() {
	try {
		conn_.unregister_onlost(this);
		if (command_thread_.joinable()) command_thread_.join();
	} catch (std::exception &e) {
		LOG_F(ERROR, "Unexpected error during destruction of an command_sender: %s", e.what());
	} catch (...) { LOG_F(ERROR, "Severe error during command sender shutdown."); }
}

const lsl::stream_info_impl &lsl::command_sender::send_commands(std::string commands, double timeout) {
	std::unique_lock<std::mutex> lock(fullinfo_mut_);

	commands_ = commands;

	if (!conn_.lost()) {
		// start thread if not yet running
		if (!command_thread_.joinable()) command_thread_ = std::thread(&command_sender::command_thread, this);
		// wait until we are ready to return a result (or we time out)
		if (timeout >= FOREVER)
			fullinfo_upd_.wait(lock);
		else if (fullinfo_upd_.wait_for(lock, std::chrono::duration<double>(timeout)) == std::cv_status::timeout)
			throw timeout_error("The info() operation timed out.");
	}
	if (conn_.lost())
		throw lost_error("The stream read by this inlet has been lost. To recover, you need to "
						 "re-resolve the source and re-create the inlet.");

	if (command_thread_.joinable()) command_thread_.join();
	
	return *fullinfo_;
}

void lsl::command_sender::command_thread() {
	conn_.acquire_watchdog();
	loguru::set_thread_name((std::string("C_") += conn_.type_info().name().substr(0, 12)).c_str());
	try {
		while (!conn_.lost() && !conn_.shutdown()) {
			try {
				// make a new stream buffer & stream
				cancellable_streambuf buffer;
				buffer.register_at(&conn_);
				std::iostream server_stream(&buffer);
				// connect...
				buffer.connect(conn_.get_tcp_endpoint());
				// send the command
				server_stream << "LSL:command\r\n" << commands_ << "\r\n" << std::flush;
				// receive and parse the response
				std::ostringstream os;
				os << server_stream.rdbuf();
				stream_info_impl info;
				std::string msg = os.str();
				info.from_fullinfo_message(msg);
				// do not retry. user must double check to make sure the info is updated properly.
				// if (!info.created_at()) continue;
				// store the result for pickup & return
				{
					std::lock_guard<std::mutex> lock(fullinfo_mut_);
					fullinfo_ = std::make_shared<stream_info_impl>(info);
				}
				fullinfo_upd_.notify_all();
				break;
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