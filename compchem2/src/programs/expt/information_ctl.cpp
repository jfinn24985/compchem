// ------------------------------------------------------------
// class: information_ctl
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/information_ctl.hpp"

namespace expt {

//Create a new DOM node, add it to the document and return an iterator to it.
//
//\post Result != end () and not (*Result.is_null ())

/**
 * Create a new DOM node, add it to the document and return an iterator to it.
 * 
 * \post Result != end () and not (*Result.is_null ())
 */
item_iterator information_ctl::create_item() {
  // DO
  is_valid ();
  purpletree::dom::element l_node (get_parent_node ().owner_document ().create_element (get_node_name ()));
  ENSURE (not l_node.is_null (), "Newly created element should not be null.");
  
  // Add to parent node
  l_node = get_parent_node ().append_child (l_node);
  
  // Indicate that document is dirty.
  journal_ctl::make_dirty (l_node.owner_document ());
  
  // Create iterator
  item_iterator Result(*this, l_node);
  // DONE
  ENSURE (Result != end (), "Iterator of new item should not equal end().");
  return Result;
}

//Get the text from the "information" node as the "value" of the information. Result may be an empty string.
//
//\pre is_valid_iterator(a_cursor) 

/**
 * Get the text from the "information" node as the "value" of the information. Result may be an empty string.
 * 
 * \pre is_valid_iterator(a_cursor) 
 */
std::string information_ctl::get_value(const item_iterator & a_cursor)
{
  require_valid_iterator (a_cursor);
  return xml_traits::get_node_content (*a_cursor);
}

/**
 * Not defined, added to stop copying.
 */
information_ctl::information_ctl(const information_ctl & source){
}

//Class local test of invariants.

/**
 * Class local test of invariants.
 */
void information_ctl::is_valid() const {
  base_t::is_valid ();
  INVARIANT (xml_traits::is_element(get_parent_node (), PARENT_NODE_NAME)
             , "List control must have a parent element or right type.");
}

//Check the iterator is valid for page_ctl

/**
 * Check the iterator is valid for page_ctl
 */
bool information_ctl::is_valid_iterator(const item_iterator & a_cursor)
{
  return xml_traits::is_element (*a_cursor, INFORMATION_NODE_NAME);
}

/**
 * Not defined, added to stop copying.
 */
information_ctl & information_ctl::operator =(const information_ctl & source) {
}

//Check the iterator is not null and contains a valid "information" element.  A valid "information" element requires a non-empty "name" attribute before it can contain a value.

/**
 * Check the iterator is not null and contains a valid "information" element.  A valid "information" element requires a non-empty "name" attribute before it can contain a value.
 */
void information_ctl::require_valid_iterator(const item_iterator & a_cursor)
{
  REQUIRE (is_valid_iterator (a_cursor), "Require iterator to 'information' type node");
  REQUIRE (not (*a_cursor).get_attribute (NAME_ATTR_NAME).empty ()
              or xml_traits::get_node_content (*a_cursor).empty ()
           , "Cannot have a value without a name.");
}

//Set the "name" attribute of the "information" node.
//
//\pre not a_string.empty()
//\pre is_valid_iterator(a_cursor)
//\pre get_name(a_cursor).empty () implies get_value(a_cursor).empty().

/**
 * Set the "name" attribute of the "information" node.
 * 
 * \pre not a_string.empty()
 * \pre is_valid_iterator(a_cursor)
 * \pre get_name(a_cursor).empty () implies get_value(a_cursor).empty().
 */
void information_ctl::set_name(item_iterator & a_cursor, const std::string & a_string)
{
  // PRE
  REQUIRE (not a_string.empty (), "Cannot set 'name' attribute to empty string");
  require_valid_iterator (a_cursor);
  if (xml_traits::set_attribute (*a_cursor, NAME_ATTR_NAME, purpletree::dom::set_utf8 (a_string)))
  {
    journal_ctl::make_dirty((*a_cursor).owner_document ());
  }
  ENSURE (a_string == get_name (a_cursor), "Real value does not equal value input to set_name.");
}

//Set the content of a node as the "value" of the "information" node.
//
//\pre a_string.empty() or not get_name(a_cursor).empty ()
//\pre is_valid_iterator(a_cursor)
//\pre get_name(a_cursor).empty () implies get_value(a_cursor).empty().

/**
 * Set the content of a node as the "value" of the "information" node.
 * 
 * \pre a_string.empty() or not get_name(a_cursor).empty ()
 * \pre is_valid_iterator(a_cursor)
 * \pre get_name(a_cursor).empty () implies get_value(a_cursor).empty().
 */
void information_ctl::set_value(item_iterator & a_cursor, const std::string & a_value)
{
  // PRE
  REQUIRE (a_value.empty () or not get_name (a_cursor).empty ()
  	 , "Cannot set 'value' if 'name' attribute is not set.");
  require_valid_iterator (a_cursor);
  // DO
  if (xml_traits::set_node_content (*a_cursor, a_value))
  {
    journal_ctl::make_dirty ((*a_cursor).owner_document ());
  }
  ENSURE (a_value == get_value (a_cursor), "Real value does not equal value input to set_value.");
}

const purpletree::dom::dom_string information_ctl::INFORMATION_NODE_NAME(purpletree::dom::set_utf8("info"));

const purpletree::dom::dom_string & information_ctl::PARENT_NODE_NAME(page_ctl::REFERENCE_NODE_NAME);

const purpletree::dom::dom_string information_ctl::NAME_ATTR_NAME(purpletree::dom::set_utf8 ("name"));


} // namespace expt
