#pragma once
#include "common.h"
#include "types.h"

/// @file inlet.h XML functions

/** @defgroup xml_ptr The lsl_xml_ptr object
 * @{
 */
// XML Attribute
/** Whether this attribute is empty. */
extern LIBLSL_C_API int32_t lsl_attribute_empty(lsl_xml_attribute_ptr a);

/** Name of the attribute. */
extern LIBLSL_C_API const char *lsl_attribute_name(lsl_xml_attribute_ptr a);

/** Value of the attribute. */
extern LIBLSL_C_API const char *lsl_attribute_value(lsl_xml_attribute_ptr a);

/** Convert the attribute value to specific types. */
extern LIBLSL_C_API const char *lsl_attribute_as_string(lsl_xml_attribute_ptr a);
extern LIBLSL_C_API int lsl_attribute_as_int(lsl_xml_attribute_ptr a);
extern LIBLSL_C_API unsigned int lsl_attribute_as_uint(lsl_xml_attribute_ptr a);
extern LIBLSL_C_API double lsl_attribute_as_double(lsl_xml_attribute_ptr a);
extern LIBLSL_C_API float lsl_attribute_as_float(lsl_xml_attribute_ptr a);
extern LIBLSL_C_API bool lsl_attribute_as_bool(lsl_xml_attribute_ptr a);

/** Set the attribute value by specific types. */
extern LIBLSL_C_API void lsl_attribute_set_name(lsl_xml_attribute_ptr a, const char *name);
extern LIBLSL_C_API void lsl_attribute_set_value_s(lsl_xml_attribute_ptr a, const char *rhs);
extern LIBLSL_C_API void lsl_attribute_set_value_i(lsl_xml_attribute_ptr a, int rhs);
extern LIBLSL_C_API void lsl_attribute_set_value_ui(lsl_xml_attribute_ptr a, unsigned int rhs);
extern LIBLSL_C_API void lsl_attribute_set_value_d(lsl_xml_attribute_ptr a, double rhs);
extern LIBLSL_C_API void lsl_attribute_set_value_f(lsl_xml_attribute_ptr a, float rhs);
extern LIBLSL_C_API void lsl_attribute_set_value_b(lsl_xml_attribute_ptr a, bool rhs);

/** Get the next sibling attribute. */
extern LIBLSL_C_API lsl_xml_attribute_ptr lsl_attribute_next_attribute(lsl_xml_attribute_ptr a);

/** Get the previous sibling attribute. */
extern LIBLSL_C_API lsl_xml_attribute_ptr lsl_attribute_previous_attribute(lsl_xml_attribute_ptr a);

// XML Tree Navigation
/** Get the first attribute of the element. */
extern LIBLSL_C_API lsl_xml_attribute_ptr lsl_first_attribute(lsl_xml_ptr e);

/** Get the last attribute of the element. */
extern LIBLSL_C_API lsl_xml_attribute_ptr lsl_last_attribute(lsl_xml_ptr e);

/** Get the first child of the element. */
extern LIBLSL_C_API lsl_xml_ptr lsl_first_child(lsl_xml_ptr e);

/** Get the last child of the element. */
extern LIBLSL_C_API lsl_xml_ptr lsl_last_child(lsl_xml_ptr e);

/** Get the next sibling in the children list of the parent node. */
extern LIBLSL_C_API lsl_xml_ptr lsl_next_sibling(lsl_xml_ptr e);

/** Get the previous sibling in the children list of the parent node. */
extern LIBLSL_C_API lsl_xml_ptr lsl_previous_sibling(lsl_xml_ptr e);

/** Get the parent node. */
extern LIBLSL_C_API lsl_xml_ptr lsl_parent(lsl_xml_ptr e);


// XML Tree Navigation by Name

/** Get a child with a specified name. */
extern LIBLSL_C_API lsl_xml_ptr lsl_child(lsl_xml_ptr e, const char *name);

/** Get an attribute with a specific name. */
extern LIBLSL_C_API lsl_xml_attribute_ptr lsl_attribute(lsl_xml_ptr e, const char *name);

/** Get the next sibling with the specified name. */
extern LIBLSL_C_API lsl_xml_ptr lsl_next_sibling_n(lsl_xml_ptr e, const char *name);

/** Get the previous sibling with the specified name. */
extern LIBLSL_C_API lsl_xml_ptr lsl_previous_sibling_n(lsl_xml_ptr e, const char *name);


// Content Queries

/** Whether this node is empty. */
extern LIBLSL_C_API int32_t lsl_empty(lsl_xml_ptr e);

/** Whether this is a text body (instead of an XML element). True both for plain char data and CData. */
extern LIBLSL_C_API int32_t  lsl_is_text(lsl_xml_ptr e);

/** Name of the element. */
extern LIBLSL_C_API const char *lsl_name(lsl_xml_ptr e);

/** Value of the element. */
extern LIBLSL_C_API const char *lsl_value(lsl_xml_ptr e);

/** Get child value (value of the first child that is text). */
extern LIBLSL_C_API const char *lsl_child_value(lsl_xml_ptr e);

/** Get child value of a child with a specified name. */
extern LIBLSL_C_API const char *lsl_child_value_n(lsl_xml_ptr e, const char *name);


// Data Modification

/// Append a child node with a given name, which has a (nameless) plain-text child with the given text value.
extern LIBLSL_C_API lsl_xml_ptr lsl_append_child_value(lsl_xml_ptr e, const char *name, const char *value);

/// Prepend a child node with a given name, which has a (nameless) plain-text child with the given text value.
extern LIBLSL_C_API lsl_xml_ptr lsl_prepend_child_value(lsl_xml_ptr e, const char *name, const char *value);

/// Set the text value of the (nameless) plain-text child of a named child node.
extern LIBLSL_C_API int32_t lsl_set_child_value(lsl_xml_ptr e, const char *name, const char *value);

/**
* Set the element's name.
* @return 0 if the node is empty (or if out of memory).
*/
extern LIBLSL_C_API int32_t lsl_set_name(lsl_xml_ptr e, const char *rhs);

/**
* Set the element's value.
* @return 0 if the node is empty (or if out of memory).
*/
extern LIBLSL_C_API int32_t lsl_set_value(lsl_xml_ptr e, const char *rhs);

/** Append an attribute element with the specified name. */
extern LIBLSL_C_API lsl_xml_attribute_ptr lsl_append_attribute(lsl_xml_ptr e, const char *name);

/** Prepend an attribute element with the specified name. */
extern LIBLSL_C_API lsl_xml_attribute_ptr lsl_prepend_attribute(lsl_xml_ptr e, const char *name);

/** Append a child element with the specified name. */
extern LIBLSL_C_API lsl_xml_ptr lsl_append_child(lsl_xml_ptr e, const char *name);

/** Prepend a child element with the specified name. */
extern LIBLSL_C_API lsl_xml_ptr lsl_prepend_child(lsl_xml_ptr e, const char *name);

/** Append a copy of the specified element as a child. */
extern LIBLSL_C_API lsl_xml_ptr lsl_append_copy(lsl_xml_ptr e, lsl_xml_ptr e2);

/** Prepend a child element with the specified name. */
extern LIBLSL_C_API lsl_xml_ptr lsl_prepend_copy(lsl_xml_ptr e, lsl_xml_ptr e2);

/** Remove an attribute with the specified name. */
extern LIBLSL_C_API void lsl_remove_attribute_n(lsl_xml_ptr e, const char *name);

/** Remove a specified attribute element. */
extern LIBLSL_C_API void lsl_remove_attribute(lsl_xml_ptr e, lsl_xml_attribute_ptr a);

/** Remove all attribute elements. */
extern LIBLSL_C_API void lsl_remove_attributes(lsl_xml_ptr e);

/** Remove a child element with the specified name. */
extern LIBLSL_C_API void lsl_remove_child_n(lsl_xml_ptr e, const char *name);

/** Remove a specified child element. */
extern LIBLSL_C_API void lsl_remove_child(lsl_xml_ptr e, lsl_xml_ptr e2);

/** Remove all child elements. */
extern LIBLSL_C_API void lsl_remove_children(lsl_xml_ptr e);

/// @}
