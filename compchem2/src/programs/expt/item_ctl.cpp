// ------------------------------------------------------------
// class: item_ctl
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/item_ctl.hpp"

namespace expt {

//Get an interator to the first item in the list.
//
//\post Result == end() or (*Result).node_name == get_node_name

/**
 * Get an interator to the first item in the list.
 * 
 * \post Result == end() or (*Result).node_name == get_node_name
 */
item_iterator item_ctl::begin() const {
  // PRE
  is_valid ();
  // LOCAL 
  purpletree::dom::node l_node(parent_node.first_child ());
  item_iterator Result (m_end); // Default is to == end()
  // PRE
  // DO
  
  // The first "item" is the first node below the parent node that
  // has a node_name == get_node_name ()
  for (
       ; not l_node.is_null () && get_node_name () != l_node.node_name ()
         ; l_node = l_node.next_sibling ()) {}; // Nothing in loop
  
  // If we found any element with the desired node_name then set Result to it.
  if (not l_node.is_null ())
  {
    // Have element
    Result = item_iterator (*this, purpletree::dom::element (l_node));
  }
  ENSURE (Result == m_end or xml_traits::is_element (*Result, get_node_name ())
  	, "Iterator should equal end() or be valid for this list type.");
  return Result;
}

/**
 * Not defined, added to stop copying.
 */
item_ctl::item_ctl(const item_ctl & source){
}

/**
 * Not defined, added to stop copying.
 */
item_ctl & item_ctl::operator =(const item_ctl & source) {
}


} // namespace expt
