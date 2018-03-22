// ------------------------------------------------------------
// class: base_view
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/base_view.hpp"

namespace expt {

//Constructor for UI application object.  a_url gives the location of the XML document containing the UI description. a_key gives the name of the window to instantiate as the top-level window. For simple applications this will only be invoked once.
//
//\pre is_readable_file (a_url) [not directly tested]
//\pre is_valid_key (a_key) [not directly tested]

/**
 * Constructor for UI application object.  a_url gives the location of the XML document containing the UI description. a_key gives the name of the window to instantiate as the top-level window. For simple applications this will only be invoked once.
 * 
 * \pre is_readable_file (a_url) [not directly tested]
 * \pre is_valid_key (a_key) [not directly tested]
 */
base_view::base_view(const base_view::string_t & a_key, const base_view::string_t & a_url) 
: rep (ui_traits::find_representation (a_key, a_url))
, change_flag (0)
{
  REQUIRE (NULL != rep, "Cannot create a view without a window.");
  is_valid ();
}

//General constructor for sub-windows.

/**
 * General constructor for sub-windows.
 */
base_view::base_view(const base_view::string_t & a_key, const base_view & a_parent) 
:  rep (ui_traits::find_representation (a_key, a_parent.get_rep ()))
, change_flag (0)
{
  REQUIRE (NULL != rep, "Cannot create a view without a window.");
  is_valid ();
}

/**
 * Not defined, added to stop copying.
 */
base_view::base_view(const base_view & source){
}

/**
 * Not defined, added to stop copying.
 */
base_view & base_view::operator =(const base_view & source) {
}


} // namespace expt
