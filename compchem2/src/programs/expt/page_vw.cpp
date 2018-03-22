// ------------------------------------------------------------
// class: page_vw
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/page_vw.hpp"

namespace expt {

//Append columns to the tree view.

/**
 * Append columns to the tree view.
 */
void page_vw::columns::set_columns(itemlist_vw::list_window_t a_tree_view) {
  a_tree_view->append_column("Page Ref.", m_pageref); 
}

//Create a new list control.

/**
 * Create a new list control.
 */
std::auto_ptr< control::item_ctl > page_vw::create_list_control(const purpletree::dom::element & a_node) const {
  return std::auto_ptr< control::item_ctl >(new control::page_ctl (a_node));
}

//[UI Callback] Called when the "Finalise" button is clicked.

/**
 * [UI Callback] Called when the "Finalise" button is clicked.
 */
void page_vw::finalise_page() {
  // DO
  CHECK (not control::page_ctl::is_done (get_cursor ()), "Cannot finalise a page that is already finalised");
  
  // If unsaved changes, save them now.
  if (is_changed ())
  {
    poll ();
  }
  
  // Finalise the page.
  control::page_ctl::finalise (get_cursor ());
  
  // Update views.
  update ();
  entry_view.update ();
  info_view.update ();
}

//This view is changed if it or it's children are changed

/**
 * This view is changed if it or it's children are changed
 */
bool page_vw::is_changed() const {
  // DO
  if (!base_t::is_changed ())
  {
    if (entry_view.is_changed () or info_view.is_changed ())
      {
        on_change ();
      }
    else
      { 
        if (is_state_four ())
  	{
  	  CHECK (title_view->get_text () == control::page_ctl::get_title (get_cursor ())
  		 , "Title field is not editable");
  	  CHECK (pageref_view->get_text () == control::page_ctl::get_pageref (get_cursor ())
  		 , "Page reference field is not editable.");
  	  CHECK (startdate_view->get_text () == ui_traits::date_to_text (control::page_ctl::get_start_date (get_cursor ()))
  		 , "Start date field is not editable.");
  	  CHECK (not control::page_ctl::is_done (get_cursor ())
  		 or enddate_view->get_text () == ui_traits::date_to_text (control::page_ctl::get_final_date (get_cursor ()))
  		 , "End date field is not editable.");
  	}
        else
  	{
  	  CHECK (title_view->get_text ().empty ()
  		 , "Title field should be empty");
  	  CHECK (pageref_view->get_text ().empty ()
  		 , "Page reference field should be empty.");
  	  CHECK (startdate_view->get_text ().empty ()
  		 , "Start date field should be empty.");
  	  CHECK (enddate_view->get_text ().empty ()
  		 , "End date field should be empty.");
  	}
      }
  }
  return base_t::is_changed ();
}

//Test invariant conditions

/**
 * Test invariant conditions
 */
void page_vw::is_valid() const {
  base_t::is_valid ();
  INVARIANT (NULL != title_view, "Unable to find title widget");
  INVARIANT (NULL != pageref_view, "Unable to find page reference widget");
  INVARIANT (NULL != startdate_view, "Unable to find start date widget");
  INVARIANT (NULL != enddate_view, "Unable to find end date widget");
  INVARIANT (NULL != finalise_button, "Unable to find finalise button");
  INVARIANT (false == enddate_view->is_sensitive ()
  	   , "End date widget must never be be sensitive");
  INVARIANT (false == pageref_view->is_sensitive ()
             , "Page reference widget must never be be sensitive"); 
  INVARIANT (false == startdate_view->is_sensitive ()
             , "Start date widget must never be be sensitive");
  INVARIANT (false == title_view->is_sensitive ()
             , "Title widget must never be be sensitive");
  INVARIANT (entry_view.is_state_one () == info_view.is_state_one ()
  	   , "Sub-views should be in state one or not at the same time.");
}

/**
 * Not defined, added to stop copying.
 */
page_vw & page_vw::operator =(const page_vw & source) {
}

page_vw::page_vw(const base_view & a_journal_view) 
: itemlist_vw (PAGE_VIEW_KEY, PAGELIST_VIEW_KEY, a_journal_view, new columns)
, entry_view (*this)
, info_view (*this)
, enddate_view (dynamic_cast <Gtk::Entry*> (ui_traits::find_representation (ENDDATE_VIEW_KEY, get_rep ())))
, finalise_button (dynamic_cast <Gtk::Button*> (ui_traits::find_representation (FINALISE_BUTTON_KEY, get_rep ())))
, pageref_view (dynamic_cast <Gtk::Entry*> (ui_traits::find_representation (PAGEREF_VIEW_KEY, get_rep ())))
, startdate_view (dynamic_cast <Gtk::Entry*> (ui_traits::find_representation (STARTDATE_VIEW_KEY, get_rep ())))
, title_view (dynamic_cast <Gtk::Entry*> (ui_traits::find_representation (TITLE_VIEW_KEY, get_rep ())))
{
  is_valid ();
  // Connect to finalise button.
  finalise_button->signal_clicked ().connect (sigc::mem_fun (this, &page_vw::finalise_page));
  
  // All display views are insensitive
  enddate_view->set_sensitive (false);
  pageref_view->set_sensitive (false);
  startdate_view->set_sensitive (false);
  title_view->set_sensitive (false);
  // Button view starts insensitive
  finalise_button->set_sensitive (false);
  // Main window starts insensitive
  get_rep()->set_sensitive (false);
}

/**
 * Not defined, added to stop copying.
 */
page_vw::page_vw(const page_vw & source){
}

//Set the values in a_row from the data pointed to by a_cursor.

/**
 * Set the values in a_row from the data pointed to by a_cursor.
 */
void page_vw::set_row(const itemlist_vw::row_iterator_t & a_row, const control::item_iterator & a_cursor) {
  // const row_iterator_t & a_row, const item_iterator & a_cursor
  // Get definition
  const columns* l_defn (dynamic_cast < const columns* > (m_columns.get ()));
  CHECK (NULL != l_defn, "Column definition is of the wrong type.");
  // Convert to ustring.
  (*a_row)[l_defn->m_pageref] = Glib::ustring(control::page_ctl::get_pageref (a_cursor));
  (*a_row)[l_defn->m_iterator] = a_cursor;
}

//Populate the newly created item in a_cursor with queried data.  The base function does nothing.

/**
 * Populate the newly created item in a_cursor with queried data.  The base function does nothing.
 */
void page_vw::virt_create_item(control::item_iterator & a_cursor) {
  // DO
  REQUIRE (not a_cursor.is_null (), "Cannot set values on a null node.");
  // LOCAL
  try
  {
    std::string l_title (control::page_ctl::get_title (a_cursor));
  std::string l_pageref (control::page_ctl::get_pageref (a_cursor));
  // DO
  const ui_traits::ask_info l_title_request ("A title for the new page (may not be empty)");
  const ui_traits::ask_info l_pageref_request ("Replace or accept page reference  (may not be empty)");
  do
  {
    ui_traits::ask_query (l_title_request, l_title);
  }
  while (l_title.empty ());
  
  do
  {
    ui_traits::ask_query (l_pageref_request, l_pageref);
  }
  while (l_pageref.empty ());
  
  // Set values.
  control::page_ctl::set_pageref (a_cursor, l_pageref);
  control::page_ctl::set_title (a_cursor, l_title);
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
void page_vw::virt_poll() {
  // DO
  if (entry_view.is_changed ())
  {
    entry_view.poll ();
  }
  
  if (info_view.is_changed ())
  {
    info_view.poll ();
  }
  
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
void page_vw::virt_update() {
  // LOCAL
  const string_t l_empty;
  // PRE
  // update() REQUIREs not is_changed
  is_valid ();
  // DO
  
  // Reset data in sub-views (both are in the same state)
  if (not entry_view.is_state_one ())
  {
    entry_view.clear_parent_node ();
    info_view.clear_parent_node ();
  }
  
  // Update data in current view and in sub-views
  if (is_state_four ())
  {
    title_view->set_text (control::page_ctl::get_title (get_cursor ()));
    pageref_view->set_text (control::page_ctl::get_pageref (get_cursor ()));
    startdate_view->set_text (ui_traits::date_to_text (control::page_ctl::get_start_date (get_cursor ())));
  
    // Set up final date and finalise button.
    if (control::page_ctl::is_done (get_cursor ()))
      {
        enddate_view->set_text (ui_traits::date_to_text (control::page_ctl::get_final_date (get_cursor ())));
        // If is_done then finalise button is not necessary
        finalise_button->set_sensitive (false);
      }
    else
      {
        enddate_view->set_text (l_empty);
        // If not is_done then finalise button is necessary
        finalise_button->set_sensitive (true);
      }
    // Pass data onto sub-views
    entry_view.set_parent_node (control::page_ctl::get_entry_parent_node (get_cursor ()));
    info_view.set_parent_node (control::page_ctl::get_information_parent_node (get_cursor ()));
  }
  else
  {
    title_view->set_text (l_empty);
    pageref_view->set_text (l_empty);
    startdate_view->set_text (l_empty);
    enddate_view->set_text (l_empty);
    finalise_button->set_sensitive (false);
  }
}

const base_view::string_t page_vw::PAGELIST_VIEW_KEY("page_list");

const base_view::string_t page_vw::PAGE_VIEW_KEY("page_view");

const base_view::string_t page_vw::TITLE_VIEW_KEY("title_entry");

const base_view::string_t page_vw::PAGEREF_VIEW_KEY("pageref_entry");

const base_view::string_t page_vw::STARTDATE_VIEW_KEY("startdate_entry");

const base_view::string_t page_vw::ENDDATE_VIEW_KEY("enddate_entry");

const base_view::string_t page_vw::FINALISE_BUTTON_KEY("finalise_button");


} // namespace expt
