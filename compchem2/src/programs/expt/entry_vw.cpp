// ------------------------------------------------------------
// class: entry_vw
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/entry_vw.hpp"

namespace expt {

//Append columns to the tree view.

/**
 * Append columns to the tree view.
 */
void entry_vw::columns::set_columns(itemlist_vw::list_window_t a_tree_view) {
  a_tree_view->append_column("Date", m_entrydate); 
}

//Create a new list control.

/**
 * Create a new list control.
 */
std::auto_ptr< control::item_ctl > entry_vw::create_list_control(const purpletree::dom::element & a_node) const {
  return std::auto_ptr< control::item_ctl >(new control::entry_ctl (a_node));
}

//PART TODO
//
//\pre a_entry != NULL

/**
 * PART TODO
 * 
 * \pre a_entry != NULL
 */
entry_vw::entry_vw(const base_view & a_journal_view)
: itemlist_vw (ENTRY_VIEW_KEY, ENTRYLIST_VIEW_KEY, a_journal_view, new columns)
, date_view (dynamic_cast< Gtk::Entry* > (ui_traits::find_representation (DATE_VIEW_KEY, get_rep ())))
, sequence_view (dynamic_cast< Gtk::Entry* > (ui_traits::find_representation (SEQUENCE_VIEW_KEY, get_rep ())))
, observation_view (dynamic_cast< Gtk::TextView* > (ui_traits::find_representation (OBSERVATION_VIEW_KEY, get_rep ())))
, action_view (dynamic_cast< Gtk::TextView* > (ui_traits::find_representation (ACTION_VIEW_KEY, get_rep ())))
{
  // PRE
  is_valid ();
  // REQUIRE (not date_view->is_editable (), "Date view should not be editable.");
  
  // DO
}

/**
 * Not defined, added to stop copying.
 */
entry_vw::entry_vw(const entry_vw & source){
}

//Are there edits to UI content that has not been passed to a control object?
//
//\post Result implies 0 /= get_change_flag ()

/**
 * Are there edits to UI content that has not been passed to a control object?
 * 
 * \post Result implies 0 /= get_change_flag ()
 */
bool entry_vw::is_changed() const {
  // PRE
  is_valid ();
  // DO
  // If a change is not set, test for changes
  if (not base_t::is_changed ())
  {
    // Test for differences
    if (is_state_four ())
      {
        CHECK (date_view->get_text () == ui_traits::date_to_text (control::entry_ctl::get_entry_date (get_cursor ()))
  	     , "Entry date text should not be editable");
        if (sequence_view->get_text () != control::entry_ctl::get_sequence (get_cursor ()))
  	{
  	  on_change ();
  	}
        else if (observation_view->get_buffer ()->get_text () != control::entry_ctl::get_observation (get_cursor ()))
  	{
  	  on_change ();
  	}
        else if (action_view->get_buffer ()->get_text () != control::entry_ctl::get_action (get_cursor ()))
  	{
  	  on_change ();
  	}
      }
    else
      {
        CHECK (date_view->get_text ().empty ()
  	     , "Entry date text should be empty.");
        CHECK (sequence_view->get_text ().empty ()
  	     , "Should have no text in entry sequence field.");
        CHECK (observation_view->get_buffer ()->get_text ().empty ()
               , "Should have no text in entry observation field.");
        CHECK (action_view->get_buffer ()->get_text ().empty ()
  	     , "Should have no text in entry action field.");
      }
  }
  return base_t::is_changed ();
}

/**
 * Not defined, added to stop copying.
 */
entry_vw & entry_vw::operator =(const entry_vw & source) {
}

//Set the values in a_row from the data pointed to by a_cursor.

/**
 * Set the values in a_row from the data pointed to by a_cursor.
 */
void entry_vw::set_row(const itemlist_vw::row_iterator_t & a_row, const control::item_iterator & a_cursor) {
  // const row_iterator_t & a_row, const item_iterator & a_cursor
  REQUIRE (not a_cursor.is_null (), "Invalid item iterator");
  // DO
  columns* l_defn = dynamic_cast< columns* >(m_columns.get ());
  CHECK (NULL != l_defn, "List has incorrect column definition.");
  // Set date
  (*a_row)[l_defn->m_entrydate] = ui_traits::date_to_text (control::entry_ctl::get_entry_date (a_cursor));
  // Set iterator.
  (*a_row)[l_defn->m_iterator] = a_cursor;
}

//Poll the view for the changed data (Called from poll)

/**
 * Poll the view for the changed data (Called from poll)
 */
void entry_vw::virt_poll() {
  // Update the control objects 
  control::entry_ctl::set_sequence (get_cursor ()
  				  , sequence_view->get_text ());
  control::entry_ctl::set_observation (get_cursor ()
  				     , observation_view->get_buffer ()->get_text ());
  control::entry_ctl::set_action (get_cursor ()
  				, action_view->get_buffer ()->get_text ());
  // Reset change flag
  reset_change_flag ();
}

//Update the display to view the current data (Creating a display if necessary)

/**
 * Update the display to view the current data (Creating a display if necessary)
 */
void entry_vw::virt_update() {
  // PRE
  is_valid ();
  // DO
  if (is_state_four ())
  {
    // Update the objects
    date_view->set_text (ui_traits::date_to_text (control::entry_ctl::get_entry_date (get_cursor ())));
    sequence_view->set_text (control::entry_ctl::get_sequence (get_cursor ()));
    observation_view->get_buffer ()->set_text (control::entry_ctl::get_observation (get_cursor ()));
    action_view->get_buffer ()->set_text (control::entry_ctl::get_action (get_cursor ()));
    // Set the sensitivity
    const bool l_not_is_done (not control::page_ctl::is_done (list_control.get ()));
    sequence_view->set_sensitive (l_not_is_done);
    observation_view->set_sensitive (l_not_is_done);
    action_view->set_sensitive (l_not_is_done);
    m_button->set_sensitive (l_not_is_done);
  }
  else
  {
    const string_t l_empty;
    // Reset the views
    date_view->set_text (l_empty);
    sequence_view->set_text (l_empty);
    observation_view->get_buffer ()->set_text (l_empty);
    action_view->get_buffer ()->set_text (l_empty);
    // Set the sensitivity
    sequence_view->set_sensitive (false);
    observation_view->set_sensitive (false);
    action_view->set_sensitive (false);
    if (not is_state_one())
      {
        m_button->set_sensitive (not control::page_ctl::is_done (list_control.get ()));
      }
  }
  // No need to reset change flag
  // change_flag = 0;
}

const base_view::string_t entry_vw::ACTION_VIEW_KEY("action_entry");

const base_view::string_t entry_vw::DATE_VIEW_KEY("date_entry");

const base_view::string_t entry_vw::ENTRY_VIEW_KEY("entry_view");

const base_view::string_t entry_vw::ENTRYLIST_VIEW_KEY("entry_list");

const base_view::string_t entry_vw::OBSERVATION_VIEW_KEY("observation_entry");

const base_view::string_t entry_vw::SEQUENCE_VIEW_KEY("sequence_entry");


} // namespace expt
