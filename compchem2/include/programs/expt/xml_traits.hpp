#ifndef XML_TRAITS_HPP
#define XML_TRAITS_HPP

// ------------------------------------------------------------
// class: xml_traits
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>

namespace expt {

/**
 * A set of XML/DOM related helper methods.
 */
class xml_traits {
  public:
    /**
     * Find the first element that has a_name starting from a_start node (inclusive).
     * 
     * \post Result.is_null () or (Result.node_name() = a_name and Result.node_type() = ELEMENT_NODE)
     */
    static purpletree::dom::node find_node_by_name(const purpletree::dom::node & a_start, const purpletree::dom::dom_string & a_name);

    /**
     * Find the last element that has a_name starting from a_start node (inclusive).
     * 
     * \post Result.is_null () or (Result.node_name() = a_name and Result.node_type() = ELEMENT_NODE)
     */
    static purpletree::dom::node find_last_node_by_name(const purpletree::dom::node & a_start, const purpletree::dom::dom_string & a_name);

    /**
     * Convert the ISO representation of a date in the attribute of a_cursor as a date.
     * 
     * \pre not a_cursor.is_null
     * \pre not a_attr_name.empty 
     * \pre not a_cursor.get_attribute (a_attr_name).empty
     * \post not Result.is_not_a_date ()
     */
    static boost::gregorian::date get_date_attribute(const purpletree::dom::element & a_cursor, const purpletree::dom::dom_string & a_attr_name);

    /**
     * Retrieve the text content of an element. It is the concatentation of all text nodes. Returned value may be an empty string
     * 
     * \pre not a_node.is_null
     */
    static std::string get_node_content(const purpletree::dom::element & a_node);

    /**
     * Is a_node a DOM element with tag of a_node_name?
     * 
     * \post Result implies: not a_node.is_null
     *                                 and a_node.node_name== a_node_name
     *                                 and a_node.node_type = ELEMENT
     */
    static inline bool is_element(const purpletree::dom::node & a_node, const purpletree::dom::dom_string & a_node_name) {
          return not a_node.is_null ()
                 and purpletree::dom::node::ELEMENT_NODE == a_node.node_type ()
                 and a_node.node_name () == a_node_name;
        };

    /**
     * Set attribute with a_attr_name of a_cursor to a_value.  If a_value is an empty string then attribute is removed. Return value indicates if attribute value was actually changed.
     * 
     * \pre not a_cursor.is_null
     * \post false implies a_value = (a_cursor_OLD).get_attribute (a_attr_name)
     * \post a_value = a_cursor.get_attribute (a_attr_name)
     */
    static bool set_attribute(purpletree::dom::element & a_cursor, const purpletree::dom::dom_string & a_attr_name, const purpletree::dom::dom_string & a_value);

    /**
     * Set an attribute of a_cursor to be a representation of a_date in ISO format.
     * 
     * \pre not a_cursor.is_null and not a_attr_name.empty
     * \pre not a_value.is_not_a_date
     * \pre a_initial implies a_cursor.get_attribute (a_attr_name).empty()
     * \post false implies a_value = get_date_attribute (a_cursor_OLD, a_attr_name)
     * \post a_value = get_date_attribute (a_cursor, a_attr_name)
     */
    static bool set_date_attribute(purpletree::dom::element & a_cursor, const purpletree::dom::dom_string & a_attr_name, const boost::gregorian::date & a_value, bool a_initial);

    /**
     * Set the text content of the node.  If any text nodes exist, the content of the first will be deleted and all other text nodes will be removed.  If no text nodes exist, one will be created and appended to the node. If a_value is empty then all content is removed.
     * 
     * \pre a_node.node_type () = ELEMENT_TYPE
     * \post a_value = get_node_content (a_node)
     */
    static bool set_node_content(purpletree::dom::element & a_node, const std::string & a_value);

};

} // namespace expt
#endif
