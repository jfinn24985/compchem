// ------------------------------------------------------------
// class: item_iterator
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/item_iterator.hpp"

namespace expt {

//Increment the iterator.

/**
 * Increment the iterator.
 */
const item_iterator & item_iterator::operator ++() {
  // LOCAL
  // PRE
  // DO
  CHECK (*this != parent->end () or cursor.is_null ()
         , "Cursor should be null when iterator = parent.end");
  // Find the next node that has a node_name == item_ctl::get_node_name ()
  if (not cursor.is_null ())
  {
    cursor = xml_traits::find_node_by_name (cursor.next_sibling ()
  					  , parent->get_node_name ());
  }
  if (cursor.is_null ())
  {
    // End of list; return end() iterator
    *this = parent->end ();
  }
  return *this;
}


} // namespace expt
