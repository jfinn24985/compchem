#ifndef INFORMATION_CTL_HPP
#define INFORMATION_CTL_HPP

// ------------------------------------------------------------
// class: information_ctl
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/expt/item_ctl.hpp"
#include "programs/expt/item_iterator.hpp"
#include <string>

namespace expt {

/**
 * A map of name/value pairs that categorise the given current object.
 * 
 * <reference>(<info name="...">...value...</info>)*</reference>
 */
class information_ctl : public item_ctl {
  public:
    typedef item_ctl base_t;

    /**
     * Create a new DOM node, add it to the document and return an iterator to it.
     * 
     * \post Result != end () and not (*Result.is_null ())
     */
    item_iterator create_item();

    /**
     * The DOM node name for the represented elements.
     * 
     * \post not Result.empty ()
     */
    inline const purpletree::dom::dom_string & get_node_name() const {
          is_valid ();
          ENSURE (not INFORMATION_NODE_NAME.empty (), "Node name of 'information' elements cannot be empty.");
          return INFORMATION_NODE_NAME;
        };

    /**
     * Get the "name" attribute of the "information" node.
     * 
     * \pre is_valid_iterator(a_cursor) 
     * \post Result.empty () implies get_value (a_cursor).empty ()
     */
    static inline std::string get_name(const item_iterator & a_cursor) {
          require_valid_iterator (a_cursor);
          return purpletree::dom::get_utf8 ((*a_cursor).get_attribute (NAME_ATTR_NAME));
        };

    /**
     * Get the text from the "information" node as the "value" of the information. Result may be an empty string.
     * 
     * \pre is_valid_iterator(a_cursor) 
     */
    static std::string get_value(const item_iterator & a_cursor);

    /**
     * Public constructor.  Construct a new object to control "information" node children of a_parent.
     * 
     * \pre not a_parent.is_null ()
     */
    inline information_ctl(const purpletree::dom::element & a_parent) : item_ctl (a_parent)
        {
          is_valid ();
        };


  private:
    /**
     * Not defined, added to stop copying.
     */
    information_ctl(const information_ctl & source);


  public:
    /**
     * Destructor, this derived class has no special actions here.
     */
    inline ~information_ctl() {};


  protected:
    /**
     * Class local test of invariants.
     */
    void is_valid() const;


  public:
    /**
     * Check the iterator is valid for page_ctl
     */
    static bool is_valid_iterator(const item_iterator & a_cursor);


  private:
    /**
     * Not defined, added to stop copying.
     */
    information_ctl & operator =(const information_ctl & source);

    /**
     * Check the iterator is not null and contains a valid "information" element.  A valid "information" element requires a non-empty "name" attribute before it can contain a value.
     */
    static void require_valid_iterator(const item_iterator & a_cursor);


  public:
    /**
     * Set the "name" attribute of the "information" node.
     * 
     * \pre not a_string.empty()
     * \pre is_valid_iterator(a_cursor)
     * \pre get_name(a_cursor).empty () implies get_value(a_cursor).empty().
     */
    static void set_name(item_iterator & a_cursor, const std::string & a_string);

    /**
     * Set the content of a node as the "value" of the "information" node.
     * 
     * \pre a_string.empty() or not get_name(a_cursor).empty ()
     * \pre is_valid_iterator(a_cursor)
     * \pre get_name(a_cursor).empty () implies get_value(a_cursor).empty().
     */
    static void set_value(item_iterator & a_cursor, const std::string & a_value);

    static const purpletree::dom::dom_string INFORMATION_NODE_NAME;

    static const purpletree::dom::dom_string & PARENT_NODE_NAME;

    static const purpletree::dom::dom_string NAME_ATTR_NAME;

};

} // namespace expt
#endif
