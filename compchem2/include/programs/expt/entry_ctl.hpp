#ifndef ENTRY_CTL_HPP
#define ENTRY_CTL_HPP

// ------------------------------------------------------------
// class: entry_ctl
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
 * A control object for the entry on a journal page.  This control maintains the current date, optional subjective observation, a decision/action, a cross-reference and/or an objective result present in an entry.
 * 
 * Modelled concepts controlled:
 * * entry_date
 * * observation
 * * action
 * * sequence (cross-reference, may be used to link objective result)
 * 
 * Future work:
 * * Only the last entry should be editable.
 * 
 * 
 * Model XML
 * 
 * <entry date="..." sequence="..."?><observation/>?<action/>?</entry>
 */
class entry_ctl : public item_ctl {
  public:
    typedef item_ctl base_t;

    /**
     * Create a new DOM node, add it to the document and return an iterator to it.
     * 
     * \post Result != end () and not (*Result.is_null ())
     */
    item_iterator create_item();

    /**
     * Construct a new object to control "entry" node children of a_parent.
     * 
     * \pre not a_parent.is_null ()
     */
    inline entry_ctl(const purpletree::dom::element & a_parent) : item_ctl (a_parent)
        {
          is_valid ();
        };

    /**
     * Destructor, this derived class has no special actions here.
     */
    inline ~entry_ctl() {};


  private:
    /**
     * Not defined, added to stop copying.
     */
    entry_ctl(const entry_ctl & source);


  public:
    /**
     * Get the text associated with the action concept in the model. This string may be empty.
     * 
     * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
     */
    static std::string get_action(const item_iterator & a_cursor);

    /**
     * Get the date associated with the current entry model.
     * 
     * \pre is_valid_iterator(a_cursor) [implies (*a_cursor).has_attribute (DATE_ATTR_NAME)]
     * \post not Result.is_not_a_date
     */
    static boost::gregorian::date get_entry_date(const item_iterator & a_cursor);

    /**
     * Return the DOM node name for the represented elements.
     * 
     * \post not Result.empty ()
     */
    inline const purpletree::dom::dom_string & get_node_name() const {
          is_valid ();
          ENSURE (not ENTRY_NODE_NAME.empty (), "Empty node name string.");
          return ENTRY_NODE_NAME;
        };

    /**
     * Get the observation in the current model.  Result string may be empty.
     * 
     * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
     */
    static std::string get_observation(const item_iterator & a_cursor);

    /**
     * Get the identifier that can be used to link log entry to some external item, for example the filename of a stored spectrum. Result may be an empty string.
     * 
     * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
     */
    static inline std::string get_sequence(const item_iterator & a_cursor) {
          require_valid_iterator (a_cursor);
          return purpletree::dom::get_utf8 ((*a_cursor).get_attribute (SEQUENCE_ATTR_NAME));
        };


  protected:
    /**
     * Class local test of invariants.
     */
    void is_valid() const;


  public:
    /**
     * Check the iterator has a valid DOM Element with name == get_node_name ()
     * 
     * \post implies not (*a_cursor).is_null () and (*a_cursor).node_name () == get_node_name ()
     */
    static bool is_valid_iterator(const item_iterator & a_cursor);


  private:
    /**
     * Not defined, added to stop copying.
     */
    entry_ctl & operator =(const entry_ctl & source);

    /**
     * Check the iterator is valid for page_ctl
     * 
     * \post (*a_cursor).node_name == ENTRY_NODE_NAME
     * \post  (*a_cursor).has_attribute (DATE_ATTR_NAME)
     * \post  (*a_cursor).has_child_with_name (ACTION_NODE_NAME)
     */
    static inline void require_valid_iterator(const item_iterator & a_cursor) {
          REQUIRE (is_valid_iterator (a_cursor), "Require iterator to 'entry' type node");
          REQUIRE (not (*a_cursor).get_attribute (DATE_ATTR_NAME).empty ()
        	 , "Invalid 'entry' node, 'date' attribute is empty.");
        };


  public:
    /**
     * Attempt to set the action associated with current entry model.  This is always successful.  The given string always replaces the current datum in the model.
     * 
     * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
     * \post a_value = get_action(a_cursor)
     */
    static void set_action(item_iterator & a_cursor, const std::string & a_value);

    /**
     * Set the value of the observation in the modelled entry.  This is always successful. The given text simply replaces the current datum.
     * 
     * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
     * \post a_value = get_observeration (a_cursor)
     */
    static void set_observation(item_iterator & a_cursor, const std::string & a_value);

    /**
     * Attempt to set the sequence string for the modelled entry.  This is always successful.  The given string replaces any existing datum.
     * 
     * ** This should be a valid cross-reference value.  This is not tested in version 1.0 **
     * 
     * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
     * \post a_value = get_sequence (a_cursor)
     */
    static void set_sequence(item_iterator & a_cursor, const std::string & a_value);

    static const purpletree::dom::dom_string ACTION_NODE_NAME;

    static const purpletree::dom::dom_string DATE_ATTR_NAME;

    static const purpletree::dom::dom_string ENTRY_NODE_NAME;

    static const purpletree::dom::dom_string & PARENT_NODE_NAME;

    static const purpletree::dom::dom_string OBSERVATION_NODE_NAME;

    static const purpletree::dom::dom_string SEQUENCE_ATTR_NAME;

};

} // namespace expt
#endif
