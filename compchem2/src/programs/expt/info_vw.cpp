// ------------------------------------------------------------
// class: info_vw
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/info_vw.hpp"

namespace expt {

//Append columns to the tree view.

/**
 * Append columns to the tree view.
 */
void info_vw::columns::set_columns(itemlist_vw::list_window_t a_tree_view) {
  a_tree_view->append_column("Name", m_name);
  a_tree_view->append_column("Value", m_value); 
}

//Create a new list control.

/**
 * Create a new list control.
 */
std::auto_ptr< control::item_ctl > info_vw::create_list_control(const purpletree::dom::element & a_node) const {
  return std::auto_ptr< control::item_ctl >(new control::information_ctl (a_node));
}

//Are there edits to UI content that has not been passed to a control object?
//
//\post Result implies 0 /= get_change_flag ()

/**
 * Are there edits to UI content that has not been passed to a control object?
 * 
 * \post Result implies 0 /= get_change_flag ()
 */
bool info_vw::is_changed() const {
  // PRE
  is_valid ();
  // DO
  // If a change is not set, test for changes
  if (not base_t::is_changed ())
  {
    // Test for differences
    if (is_state_four ())
      {
        if (name_view->get_text () != control::information_ctl::get_name (get_cursor ()))
  	{
  	  on_change ();
  	}
        else if (value_view->get_text () != control::information_ctl::get_value (get_cursor ()))
  	{
  	  on_change ();
  	}
      }
    else
      {
        CHECK (name_view->get_text ().empty ()
  	     , "Should have no text in information name field.");
        CHECK (value_view->get_text ().empty ()
               , "Should have no text in information value field.");
      }
  }
  return base_t::is_changed ();
}

//Main constructor.

/**
 * Main constructor.
 */
info_vw::info_vw(const base_view & a_view)
: itemlist_vw (INFO_VIEW_KEY, INFOLIST_VIEW_KEY, a_view, new columns)
, name_view (dynamic_cast< Gtk::Entry* > (ui_traits::find_representation (NAME_VIEW_KEY, get_rep ())))
, value_view (dynamic_cast< Gtk::Entry* > (ui_traits::find_representation (VALUE_VIEW_KEY, get_rep ())))
{
  // PRE
  is_valid ();

}

/**
 * Not defined, added to stop copying.
 */
info_vw::info_vw(const info_vw & source){
}

/**
 * Not defined, added to stop copying.
 */
info_vw & info_vw::operator =(const info_vw & source) {
}

//Set the values in a_row from the data pointed to by a_cursor.

/**
 * Set the values in a_row from the data pointed to by a_cursor.
 */
void info_vw::set_row(const itemlist_vw::row_iterator_t & a_row, const control::item_iterator & a_cursor) {
  // const row_iterator_t & a_row, const item_iterator & a_cursor
  is_valid ();
  REQUIRE (not a_cursor.is_null (), "Invalid item iterator");
  // Convert from datum
  // DO
  columns *l_defn = dynamic_cast< columns* >(m_columns.get ());
  CHECK (NULL != l_defn, "Incorrect column definition.");
  // Add to tree viev
  
  (*a_row)[l_defn->m_name] = Glib::ustring(control::information_ctl::get_name (a_cursor));
  (*a_row)[l_defn->m_value] = Glib::ustring(control::information_ctl::get_value (a_cursor));
  // Set iterator.            
  (*a_row)[l_defn->m_iterator] = a_cursor;
}

//Populate the newly created item in a_cursor with queried data.  The base function does nothing, derived types that need to perform extra operations to ensure a valid data item should do so here.

/**
 * Populate the newly created item in a_cursor with queried data.  The base function does nothing, derived types that need to perform extra operations to ensure a valid data item should do so here.
 */
void info_vw::virt_create_item(control::item_iterator & a_cursor) {
  // DO
  REQUIRE (not a_cursor.is_null (), "Cannot set values on a null node.");
  // LOCAL
  try
  {
    std::string l_name (control::information_ctl::get_name (a_cursor));
    // DO
    const ui_traits::ask_info l_name_request ("A name for the current information (may not be empty)");
    do
      {
        ui_traits::ask_query (l_name_request, l_name);
      }
    while (l_name.empty ());
  
    // Set values.
    control::information_ctl::set_name (a_cursor, l_name);
  }
  catch (const std::exception &a_err)
  {
    ALWAYS(false, a_err.what ());
  }
}

//Poll the view for the changed data (Called from poll)

/**
 * Poll the view for the changed data (Called from poll)
 */
void info_vw::virt_poll() {
  // PRE
  is_valid ();
  // We know is_changed
  try
  {
    std::string l_name (name_view->get_text ());
    if (l_name.empty ())
      {
        // DO
        const ui_traits::ask_info l_name_request ("A name for the current information (may not be empty)"
  						, "Enter text for name or do you want to delete the entry?"
  						, "Accept"
  						, std::string()
  						, "Cancel Edits");
        do
  	{
  	  unsigned int l_ask (ui_traits::ask_query (l_name_request, l_name));
  // 	  if (ui_traits::NO == l_ask)
  // 	    {
  // 	      // Set for delete
  // 	      // TODO : NO DELETE MECHANISM!!
  // 	      // - remove entry from model and control
  // 	      // ** delete_row ()
  // 	      l_name.clear ();
  // 	      break;
  // 	    }
  // 	  else
  	  if (ui_traits::CANCEL == l_ask)
  	    {
  	      // Update the objects
  	      name_view->set_text (control::information_ctl::get_name (get_cursor ()));
  	      value_view->set_text (control::information_ctl::get_value (get_cursor ()));
  	      // Clear l_name
  	      l_name.clear ();
  	      break;
  	    }
  	}
        while (l_name.empty ());
      }
    // If l_name is not empty then we want to save the datum.
    if (not l_name.empty ())
      {
        // Set values.
        control::information_ctl::set_name (get_cursor (), l_name);
        control::information_ctl::set_value (get_cursor ()
  					   , value_view->get_text ());
        
        set_row (find_row (get_cursor ()), get_cursor ());
      }
  }
  catch (const std::exception &a_err)
  {
    ALWAYS(false, a_err.what ());
  }
  
  // Reset change flag.
  reset_change_flag ();
}

//Update the display to view the current data (Creating a display if necessary)
//
//\pre false = is_changed ().

/**
 * Update the display to view the current data (Creating a display if necessary)
 * 
 * \pre false = is_changed ().
 */
void info_vw::virt_update() {
  // PRE
  is_valid ();
  // update REQUIREs not is_changed ()
  // DO
  if (is_state_four ())
  {
    // Update the objects
    name_view->set_text (control::information_ctl::get_name (get_cursor ()));
    value_view->set_text (control::information_ctl::get_value (get_cursor ()));
    // Update the sensitivity
    const bool l_not_is_done (not control::page_ctl::is_done (list_control.get ()));
    name_view->set_sensitive (l_not_is_done);
    value_view->set_sensitive (l_not_is_done);
    m_button->set_sensitive (l_not_is_done);
  }
  else
  {
    const string_t l_empty;
    // Reset view content
    name_view->set_text (l_empty);
    value_view->set_text (l_empty);
    // Update the sensitivity
    name_view->set_sensitive (false);
    value_view->set_sensitive (false);
    if (not is_state_one ())
      {
        m_button->set_sensitive (not control::page_ctl::is_done (list_control.get ()));
      }
  }
}

const base_view::string_t info_vw::INFOLIST_VIEW_KEY("info_list");

const base_view::string_t info_vw::INFO_VIEW_KEY("info_view");

const base_view::string_t info_vw::NAME_VIEW_KEY("name_entry");

const base_view::string_t info_vw::VALUE_VIEW_KEY("value_entry");


} // namespace expt
