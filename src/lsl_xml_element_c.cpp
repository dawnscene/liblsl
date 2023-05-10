#include <pugixml.hpp>

extern "C" {
#include "api_types.hpp"
// include api_types before public API header
#include "../include/lsl/xml.h"

using namespace pugi;

/*************************************************************************/
/* xml_attribute                                                         */
/*************************************************************************/
LIBLSL_C_API int32_t lsl_attribute_empty(lsl_xml_attribute_ptr a) { return xml_attribute(a).empty(); }
LIBLSL_C_API const char *lsl_attribute_name(lsl_xml_attribute_ptr a) { return xml_attribute(a).name(); }
LIBLSL_C_API const char *lsl_attribute_value(lsl_xml_attribute_ptr a) { return xml_attribute(a).value(); }
LIBLSL_C_API const char *lsl_attribute_as_string(lsl_xml_attribute_ptr a) { return xml_attribute(a).as_string(); }
LIBLSL_C_API int lsl_attribute_as_int(lsl_xml_attribute_ptr a) { return xml_attribute(a).as_int(); }
LIBLSL_C_API unsigned int lsl_attribute_as_uint(lsl_xml_attribute_ptr a) { return xml_attribute(a).as_uint(); }
LIBLSL_C_API double lsl_attribute_as_double(lsl_xml_attribute_ptr a) { return xml_attribute(a).as_double(); }
LIBLSL_C_API float lsl_attribute_as_float(lsl_xml_attribute_ptr a) { return xml_attribute(a).as_float(); }
LIBLSL_C_API bool lsl_attribute_as_bool(lsl_xml_attribute_ptr a) { return xml_attribute(a).as_bool(); }

LIBLSL_C_API void lsl_attribute_set_name(lsl_xml_attribute_ptr a, const char *name) {
	xml_attribute(a).set_name(name);
}
LIBLSL_C_API void lsl_attribute_set_value_s(lsl_xml_attribute_ptr a, const char *rhs) {
	xml_attribute(a).set_value(rhs);
}
LIBLSL_C_API void lsl_attribute_set_value_i(lsl_xml_attribute_ptr a, int rhs) {
	xml_attribute(a).set_value(rhs);
}
LIBLSL_C_API void lsl_attribute_set_value_ui(lsl_xml_attribute_ptr a, unsigned int rhs) {
	xml_attribute(a).set_value(rhs);
}
LIBLSL_C_API void lsl_attribute_set_value_d(lsl_xml_attribute_ptr a, double rhs) {
	xml_attribute(a).set_value(rhs);
}
LIBLSL_C_API void lsl_attribute_set_value_f(lsl_xml_attribute_ptr a, float rhs) {
	xml_attribute(a).set_value(rhs);
}
LIBLSL_C_API void lsl_attribute_set_value_b(lsl_xml_attribute_ptr a, bool rhs) {
	xml_attribute(a).set_value(rhs);
}
LIBLSL_C_API lsl_xml_attribute_ptr lsl_attribute_next_attribute(lsl_xml_attribute_ptr a) {
	return xml_attribute(a).next_attribute().internal_object();
}
LIBLSL_C_API lsl_xml_attribute_ptr lsl_attribute_previous_attribute(lsl_xml_attribute_ptr a) {
	return xml_attribute(a).previous_attribute().internal_object();
}

/*************************************************************************/
/* xml_node                                                              */
/*************************************************************************/
LIBLSL_C_API lsl_xml_attribute_ptr lsl_first_attribute(lsl_xml_ptr e) {
	return xml_node(e).first_attribute().internal_object();
}
LIBLSL_C_API lsl_xml_attribute_ptr lsl_last_attribute(lsl_xml_ptr e) {
	return xml_node(e).last_attribute().internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_first_child(lsl_xml_ptr e) {
	return xml_node(e).first_child().internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_last_child(lsl_xml_ptr e) {
	return xml_node(e).last_child().internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_next_sibling(lsl_xml_ptr e) {
	return xml_node(e).next_sibling().internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_previous_sibling(lsl_xml_ptr e) {
	return xml_node(e).previous_sibling().internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_parent(lsl_xml_ptr e) {
	return xml_node(e).parent().internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_child(lsl_xml_ptr e, const char *name) {
	return xml_node(e).child(name).internal_object();
}
LIBLSL_C_API lsl_xml_attribute_ptr lsl_attribute(lsl_xml_ptr e, const char *name) {
	return xml_node(e).attribute(name).internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_next_sibling_n(lsl_xml_ptr e, const char *name) {
	return xml_node(e).next_sibling(name).internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_previous_sibling_n(lsl_xml_ptr e, const char *name) {
	return xml_node(e).previous_sibling(name).internal_object();
}

LIBLSL_C_API int32_t lsl_empty(lsl_xml_ptr e) { return xml_node(e).empty(); }
LIBLSL_C_API int32_t lsl_is_text(lsl_xml_ptr e) { return xml_node(e).type() != node_element; }
LIBLSL_C_API const char *lsl_name(lsl_xml_ptr e) { return xml_node(e).name(); }
LIBLSL_C_API const char *lsl_value(lsl_xml_ptr e) { return xml_node(e).value(); }
LIBLSL_C_API const char *lsl_child_value(lsl_xml_ptr e) { return xml_node(e).child_value(); }
LIBLSL_C_API const char *lsl_child_value_n(lsl_xml_ptr e, const char *name) {
	return xml_node(e).child_value(name);
}

LIBLSL_C_API int32_t lsl_set_name(lsl_xml_ptr e, const char *rhs) {
	return xml_node(e).set_name(rhs);
}
LIBLSL_C_API int32_t lsl_set_value(lsl_xml_ptr e, const char *rhs) {
	return xml_node(e).set_value(rhs);
}
LIBLSL_C_API lsl_xml_attribute_ptr lsl_append_attribute(lsl_xml_ptr e, const char *name) {
	return xml_node(e).append_attribute(name).internal_object();
}
LIBLSL_C_API lsl_xml_attribute_ptr lsl_prepend_attribute(lsl_xml_ptr e, const char *name) {
	return xml_node(e).prepend_attribute(name).internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_append_child(lsl_xml_ptr e, const char *name) {
	return xml_node(e).append_child(name).internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_prepend_child(lsl_xml_ptr e, const char *name) {
	return xml_node(e).prepend_child(name).internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_append_copy(lsl_xml_ptr e, lsl_xml_ptr e2) {
	return xml_node(e).append_copy(xml_node(e2)).internal_object();
}
LIBLSL_C_API lsl_xml_ptr lsl_prepend_copy(lsl_xml_ptr e, lsl_xml_ptr e2) {
	return xml_node(e).prepend_copy(xml_node(e2)).internal_object();
}
LIBLSL_C_API void lsl_remove_attribute_n(lsl_xml_ptr e, const char *name) {
	xml_node(e).remove_attribute(name);
}
LIBLSL_C_API void lsl_remove_attribute(lsl_xml_ptr e, lsl_xml_attribute_ptr a) {
	xml_node(e).remove_attribute(xml_attribute(a));
}
LIBLSL_C_API void lsl_remove_attributes(lsl_xml_ptr e) {
	xml_node(e).remove_attributes();
}
LIBLSL_C_API void lsl_remove_child_n(lsl_xml_ptr e, const char *name) {
	xml_node(e).remove_child(name);
}
LIBLSL_C_API void lsl_remove_child(lsl_xml_ptr e, lsl_xml_ptr e2) {
	xml_node(e).remove_child(xml_node(e2));
}
LIBLSL_C_API void lsl_remove_children(lsl_xml_ptr e) {
	xml_node(e).remove_children();
}

LIBLSL_C_API int32_t lsl_set_child_value(lsl_xml_ptr e, const char *name, const char *value) {
	return xml_node(e).child(name).first_child().set_value(value);
}

LIBLSL_C_API lsl_xml_ptr lsl_append_child_value(
	lsl_xml_ptr e, const char *name, const char *value) {
	xml_node result = xml_node(e).append_child(name);
	result.append_child(node_pcdata).set_value(value);
	return e;
}

LIBLSL_C_API lsl_xml_ptr lsl_prepend_child_value(
	lsl_xml_ptr e, const char *name, const char *value) {
	xml_node result = xml_node(e).prepend_child(name);
	result.append_child(node_pcdata).set_value(value);
	return e;
}
}
