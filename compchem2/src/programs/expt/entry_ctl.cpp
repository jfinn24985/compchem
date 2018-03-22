// ------------------------------------------------------------
// class: entry_ctl
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/entry_ctl.hpp"

namespace expt {

//Create a new DOM node, add it to the document and return an iterator to it.
//
//\post Result != end () and not (*Result.is_null ())

/**
 * Create a new DOM node, add it to the document and return an iterator to it.
 * 
 * \post Result != end () and not (*Result.is_null ())
 */
item_iterator entry_ctl::create_item() {
  // LOCAL
  // PRE
  is_valid ();
  // DO
  
  // Create new node
  purpletree::dom::element l_node (get_parent_node ().owner_document ().create_element (get_node_name ()));
  ENSURE (not l_node.is_null (), "Cannot create a new element");
  
  // Create any required sub-nodes and attributes to give a valid twig.
  // Set start date
  xml_traits::set_date_attribute (l_node
  				, DATE_ATTR_NAME
  				, boost::gregorian::day_clock::local_day()
  				, true);
        
  // Add to parent node
  l_node = get_parent_node ().append_child (l_node);
  
  // Indicate that document is dirty.
  journal_ctl::make_dirty (l_node.owner_document ());
  
  // Create iterator
  item_iterator Result = item_iterator (*this, l_node);
  
  // DONE
  ENSURE (Result != end (), "Iterator should not equal end ().");
  return Result;
}

/**
 * Not defined, added to stop copying.
 */
entry_ctl::entry_ctl(const entry_ctl & source){
}

//Get the text associated with the action concept in the model. This string may be empty.
//
//\pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)

/**
 * Get the text associated with the action concept in the model. This string may be empty.
 * 
 * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
 */
std::string entry_ctl::get_action(const item_iterator & a_cursor)
{
  // PRE
  require_valid_iterator (a_cursor);
  // DO
  std::string Result;
  purpletree::dom::element l_elem (xml_traits::find_node_by_name ((*a_cursor).first_child ()
  								, ACTION_NODE_NAME));
  if (not l_elem.is_null ())
  {
    Result = xml_traits::get_node_content (l_elem);
  }
  return Result;
}

//Get the date associated with the current entry model.
//
//\pre is_valid_iterator(a_cursor) [implies (*a_cursor).has_attribute (DATE_ATTR_NAME)]
//\post not Result.is_not_a_date

/**
 * Get the date associated with the current entry model.
 * 
 * \pre is_valid_iterator(a_cursor) [implies (*a_cursor).has_attribute (DATE_ATTR_NAME)]
 * \post not Result.is_not_a_date
 */
boost::gregorian::date entry_ctl::get_entry_date(const item_iterator & a_cursor)
{
  // PRE
  require_valid_iterator (a_cursor);
  // DO
  boost::gregorian::date Result (xml_traits::get_date_attribute (*a_cursor
  							       , DATE_ATTR_NAME));
  ENSURE (not Result.is_not_a_date ()
  	, "Invalid 'entry' node, 'date' attribute is invalid.");
  return Result;
}

//Get the observation in the current model.  Result string may be empty.
//
//\pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)

/**
 * Get the observation in the current model.  Result string may be empty.
 * 
 * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
 */
std::string entry_ctl::get_observation(const item_iterator & a_cursor)
{
  // PRE
  require_valid_iterator (a_cursor);
  // DO
  std::string Result;
  purpletree::dom::element l_elem (xml_traits::find_node_by_name ((*a_cursor).first_child ()
  								, OBSERVATION_NODE_NAME));
  if (not l_elem.is_null ())
  {
    Result = xml_traits::get_node_content (l_elem);
  }
  return Result;
}

//Class local test of invariants.

/**
 * Class local test of invariants.
 */
void entry_ctl::is_valid() const {
  base_t::is_valid ();
  INVARIANT (xml_traits::is_element(get_parent_node (), PARENT_NODE_NAME)
             , "List control must have a parent element or right type.");
}

//Check the iterator has a valid DOM Element with name == get_node_name ()
//
//\post implies not (*a_cursor).is_null () and (*a_cursor).node_name () == get_node_name ()

/**
 * Check the iterator has a valid DOM Element with name == get_node_name ()
 * 
 * \post implies not (*a_cursor).is_null () and (*a_cursor).node_name () == get_node_name ()
 */
bool entry_ctl::is_valid_iterator(const item_iterator & a_cursor)
{
  return xml_traits::is_element (*a_cursor, ENTRY_NODE_NAME);
}

/**
 * Not defined, added to stop copying.
 */
entry_ctl & entry_ctl::operator =(const entry_ctl & source) {
}

//Attempt to set the action associated with current entry model.  This is always successful.  The given string always replaces the current datum in the model.
//
//\pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
//\post a_value = get_action(a_cursor)

/**
 * Attempt to set the action associated with current entry model.  This is always successful.  The given string always replaces the current datum in the model.
 * 
 * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
 * \post a_value = get_action(a_cursor)
 */
void entry_ctl::set_action(item_iterator & a_cursor, const std::string & a_value)
{
  // PRE
  require_valid_iterator (a_cursor);
  
  // DO
  purpletree::dom::element l_node (xml_traits::find_node_by_name ((*a_cursor).first_child ()
  								, ACTION_NODE_NAME));
  
  // Is there no node but a new value?
  if (l_node.is_null () and not a_value.empty ())
  {
    // There is no existing node, we need to add one.
    // An action node is at the end of the entry node.
    l_node = (*a_cursor).append_child ((*a_cursor).owner_document ().create_element (ACTION_NODE_NAME));
    CHECK (not l_node.is_null (), "Newly created element should not be null.");
  }
  // If there is a node
  if (not l_node.is_null ())
  {
    if (xml_traits::set_node_content (l_node, a_value))
      {
        journal_ctl::make_dirty ((*a_cursor).owner_document ());
      }
  }
  ENSURE (a_value == get_action (a_cursor), "Real value does not equal value input to set_action");
}

//Set the value of the observation in the modelled entry.  This is always successful. The given text simply replaces the current datum.
//
//\pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
//\post a_value = get_observeration (a_cursor)

/**
 * Set the value of the observation in the modelled entry.  This is always successful. The given text simply replaces the current datum.
 * 
 * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
 * \post a_value = get_observeration (a_cursor)
 */
void entry_ctl::set_observation(item_iterator & a_cursor, const std::string & a_value)
{
  require_valid_iterator (a_cursor);
  // DO
  purpletree::dom::element l_node (xml_traits::find_node_by_name ((*a_cursor).first_child (), OBSERVATION_NODE_NAME));
  purpletree::dom::document l_doc ((*a_cursor).owner_document ());
  // Test if new node is required.
  if (l_node.is_null () and not a_value.empty ())
  {
    // There is no existing node, we need to add one.
    // An observation node is the first entry.
    l_node = (*a_cursor).first_child ();
    if (l_node.is_null ())
      {
        // No other children so can append
        l_node = (*a_cursor).append_child (l_doc.create_element (OBSERVATION_NODE_NAME));
      }
    else
      {
        // Prepend to first child.
        l_node = (*a_cursor).insert_before (l_doc.create_element (OBSERVATION_NODE_NAME), l_node);
      }
    CHECK (not l_node.is_null (), "Newly created element should not be null");
  }
  if (not l_node.is_null ())
  {
    if (xml_traits::set_node_content (l_node, a_value))
      {
        journal_ctl::make_dirty (l_doc);
      }
  }
  ENSURE (a_value == get_observation (a_cursor), "Real value does not equal value input to set_observation.");
}

//Attempt to set the sequence string for the modelled entry.  This is always successful.  The given string replaces any existing datum.
//
//** This should be a valid cross-reference value.  This is not tested in version 1.0 **
//
//\pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
//\post a_value = get_sequence (a_cursor)

/**
 * Attempt to set the sequence string for the modelled entry.  This is always successful.  The given string replaces any existing datum.
 * 
 * ** This should be a valid cross-reference value.  This is not tested in version 1.0 **
 * 
 * \pre is_valid_iterator(a_cursor) and (*a_cursor).has_attribute (DATE_ATTR_NAME)
 * \post a_value = get_sequence (a_cursor)
 */
void entry_ctl::set_sequence(item_iterator & a_cursor, const std::string & a_value)
{
  // PRE
  require_valid_iterator (a_cursor);
  // DO
  if (xml_traits::set_attribute (*a_cursor
  			       , SEQUENCE_ATTR_NAME
  			       , purpletree::dom::set_utf8 (a_value)))
  {
    journal_ctl::make_dirty ((*a_cursor).owner_document ());
  }
  ENSURE (a_value == get_sequence (a_cursor), "Real value does not equal input value to set_sequence.");
}

const purpletree::dom::dom_string entry_ctl::ACTION_NODE_NAME(purpletree::dom::set_utf8("action"));

const purpletree::dom::dom_string entry_ctl::DATE_ATTR_NAME(purpletree::dom::set_utf8("date"));

const purpletree::dom::dom_string entry_ctl::ENTRY_NODE_NAME(purpletree::dom::set_utf8("entry"));

const purpletree::dom::dom_string & entry_ctl::PARENT_NODE_NAME(page_ctl::LOG_NODE_NAME);

const purpletree::dom::dom_string entry_ctl::OBSERVATION_NODE_NAME(purpletree::dom::set_utf8("observation"));

const purpletree::dom::dom_string entry_ctl::SEQUENCE_ATTR_NAME(purpletree::dom::set_utf8("sequence"));


} // namespace expt
