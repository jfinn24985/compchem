// ------------------------------------------------------------
// class: itemlist_vw
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/itemlist_vw.hpp"

namespace expt {

//Save any unsaved data and remove all data from the display and desensitize the widgets.  Then discard the control object.
//
//\pre is_state_two () or is_state_three ()
//\post is_state_one ()

/**
 * Save any unsaved data and remove all data from the display and desensitize the widgets.  Then discard the control object.
 * 
 * \pre is_state_two () or is_state_three ()
 * \post is_state_one ()
 */
void itemlist_vw::clear_parent_node() {
  // PRE
  is_valid ();
  REQUIRE (not is_state_one ()
  	 , "Cannot clear parent in state one.");
  // DO
  
  // Unselect all items (This will call select_item)
  if (cursor != list_control->end ())
  {
    // Unselect all items. This will call poll() and update() view via
    // select_item.
    m_list->get_selection ()->unselect_all ();
  }
  CHECK (cursor == list_control->end ()
         , "Unselect all should call select_item.");
  
  // BEGIN ATOMIC 
  // Empty the model content
  m_model->clear ();
  
  // Delete the list_control object
  list_control.reset ();
  // END ATOMIC
  
  // Update the view
  update ();
  
  // Ensure we are now in state one.
  ENSURE (is_state_one (), "A view with no parent must be in state one.");
}

//[UI Callback] Create a new item and add it to the list.  Set the current item view to this item.

/**
 * [UI Callback] Create a new item and add it to the list.  Set the current item view to this item.
 */
void itemlist_vw::create_new_item() {
  // LOCAL
  // PRE
  is_valid ();
  REQUIRE (not is_state_one (), "Cannot add content to inactive object");
  // DO
  try
  {
    // Save any changed datum
    if (is_changed ())
      {
        CHECK (not is_state_two (), "Cannot have changed data in state two.");
        poll ();
      }
  
    // BEGIN ATOMIC
  
    // Create the new item.
    cursor = list_control->create_item ();
    CHECK (not cursor.is_null ()
  	 , "New iterator should never be null.");
    CHECK (cursor.get_parent () == list_control.get ()
  	 , "New iterator is not from current list control.");
  
    // Fill new item with data.
    virt_create_item (cursor);
  
    // Add a new row to model.
    row_iterator_t l_row = m_model->append ();
  
    // Set values in the new row.
    set_row (l_row, cursor);
  
    // Select new item.
    m_list->get_selection ()->select (l_row);
    // END ATOMIC
  
    CHECK (is_state_four (), "Should be in state four after creating an item.");
  
    // Update the display
    update ();
  }
  catch (const std::exception &a_err)
  {
    ALWAYS(false, a_err.what ());
  }
  
  // DONE
  is_valid ();
}

//Find the corresponding row iterator for a_cursor.
//
//\pre is_state_three or is_state_four
//\pre a_cursor.get_parent = list_control
//\post Result is a valid row

/**
 * Find the corresponding row iterator for a_cursor.
 * 
 * \pre is_state_three or is_state_four
 * \pre a_cursor.get_parent = list_control
 * \post Result is a valid row
 */
itemlist_vw::row_iterator_t itemlist_vw::find_row(const control::item_iterator & a_cursor) {
  // PRE 
  REQUIRE (is_state_four () or is_state_three ()
  	 , "Can only get rows in state three or four.");
  REQUIRE (a_cursor.get_parent () == list_control.get ()
  	 , "Input iterator should belong to the current list.");
  // LOCAL
  // From first row in model.
  row_iterator_t Result (m_model->children ().begin ());
  // DO
  while ((*Result)[m_columns->m_iterator] != a_cursor)
  {
    CHECK (Result != m_model->children ().end ()
  	 , "Given iterator is not in list");
    ++Result;
  }
  return Result;
}

//Test invariant conditions.
//
//\inv m_columns /= nul, m_model /= nul, m_list /= nul, m_button /= nul
//\inv not is_state_one --> list_control /= nul
//                   , cursor /= nul or cursor == list_control.end
//                   , m_list->model = m_model
//\inv is_state_one --> list_control = nul, cursor = nul

/**
 * Test invariant conditions.
 * 
 * \inv m_columns /= nul, m_model /= nul, m_list /= nul, m_button /= nul
 * \inv not is_state_one --> list_control /= nul
 *                    , cursor /= nul or cursor == list_control.end
 *                    , m_list->model = m_model
 * \inv is_state_one --> list_control = nul, cursor = nul
 */
void itemlist_vw::is_valid() const {
  base_t::is_valid ();
  INVARIANT (NULL != m_columns.get (), "Require a valid column definition");
  INVARIANT (true == m_model, "Require a valid model");
  INVARIANT (NULL != m_list, "Incorrect window type for list.");
  INVARIANT (NULL != m_button, "List needs a create item button");
  INVARIANT (is_state_one () xor is_state_two () xor is_state_three () xor is_state_four ()
  	   , "View must be in one of the defined states.");
  INVARIANT (m_list->get_model () == m_model
  	   , "View and object model must match when object is active.");
  if (NULL != list_control.get ())
  {
    INVARIANT (not cursor.is_null () or cursor == list_control->end ()
  	     , "An active object needs a valid cursor.");
  }
  else
  {
    INVARIANT (m_model->children ().empty ()
  	     , "An inactive object must have an empty model.");
    INVARIANT (cursor.is_null ()
  	     , "An inactive object must have a null cursor.");
  }
}

//Create an object to manage a view with a list.

/**
 * Create an object to manage a view with a list.
 */
itemlist_vw::itemlist_vw(const base_view::string_t & a_key, const base_view::string_t & a_list_key, const base_view & a_window, itemlist_vw::base_columns * a_column) 
: base_view (a_key, a_window)
, list_control ()
, m_model ()
, m_columns (a_column)
, cursor ()
, m_list (dynamic_cast<list_window_t> (ui_traits::find_representation (a_list_key, get_rep ())))
, m_button (dynamic_cast<button_t> (ui_traits::find_representation (a_list_key + "_button", get_rep ())))
{
  // PRE
  REQUIRE (NULL != a_column, "Require a non-null column definition.");
  REQUIRE (NULL != m_button, "Found window for button was not of right type.");
  REQUIRE (NULL != m_list, "Found window for list was not of right type."); 
  // Test window state is valid.
  REQUIRE (Gtk::SELECTION_SINGLE == m_list->get_selection ()->get_mode ()
  	 , "View window should be set to single select mode.");
  
  //DO
  m_model = Gtk::ListStore::create (*a_column);
  
  // Connect the button signal.
  m_button->signal_clicked ().connect (sigc::mem_fun (this
  						    , &itemlist_vw::create_new_item));
  
  // Connect the select signal.
  m_list->get_selection ()->signal_changed ().connect (sigc::mem_fun (this
  								    , &itemlist_vw::select_item));
  
  // Connect model to tree
  m_list->set_model (m_model);
  
  // Add column viewer
  m_columns->set_columns (m_list);
  
  // Set the sensitivity
  m_button->set_sensitive (false);
  m_list->set_sensitive (false);
  get_rep ()->set_sensitive (false);
  
  // Check we have a valid object
  is_valid ();
}

/**
 * Not defined, added to stop copying.
 */
itemlist_vw::itemlist_vw(const itemlist_vw & source){
}

/**
 * Not defined, added to stop copying.
 */
itemlist_vw & itemlist_vw::operator =(const itemlist_vw & source) {
}

//[UI Callback] This method is called (from the UI objects) to indicate that the selection in the UI object has been changed.

/**
 * [UI Callback] This method is called (from the UI objects) to indicate that the selection in the UI object has been changed.
 */
void itemlist_vw::select_item() {
  // Only be concerned when there are items in the model.
  if (is_state_four () or is_state_three ())
  {
    // PRE
    is_valid ();
    // DO
    Glib::RefPtr<Gtk::TreeSelection> l_selection (m_list->get_selection ());
    
    CHECK (1 == l_selection->count_selected_rows () 
  	 or 0 == l_selection->count_selected_rows ()
  	 , "Can only have one or no rows selected.");
    
    // If there is no selection unset_cursor otherwise get the selected
    // row, get iterator from row and then set_cursor.
    control::item_iterator l_cursor;
    if (0 == l_selection->count_selected_rows ())
      {
        // No selection, set to list_control->end
        l_cursor = list_control->end ();
      }
    else
      {
        // Convert selection to item_iterator
        l_cursor = find_row (l_selection->get_selected ());
      }
  
    // Only do something if the cursor has changed
    if (l_cursor != cursor)
      {
        if (is_changed ())
  	{
  	  poll ();
  	}
        cursor = l_cursor;
        update ();
      }
  }
}

//Set a_node as the data in the display and make the displayed objects sensitive.
//
//\pre is_state_one()
//\pre not a_node.is_null ()
//\post is_state_two () or is_state_three ()
//\post not is_changed ()

/**
 * Set a_node as the data in the display and make the displayed objects sensitive.
 * 
 * \pre is_state_one()
 * \pre not a_node.is_null ()
 * \post is_state_two () or is_state_three ()
 * \post not is_changed ()
 */
void itemlist_vw::set_parent_node(const purpletree::dom::element & a_node) {
  // PRE
  is_valid ();
  REQUIRE (is_state_one (), "Can only set parent on node is state one.");
  REQUIRE (not a_node.is_null (), "Cannot set parent to a null element.");
  // DO
  // The model should be empty now.
  CHECK (true == m_model->children ().empty (), "List should be empty!");
  
  // BEGIN ATOMIC
  
  // Create a new list_control with the new node.
  list_control = create_list_control (a_node);
  ENSURE (NULL != list_control.get ()
  	, "Setting a parent should give a valid list control object.");
  // Set cursor to an iterator from the new list_control
  cursor = list_control->end ();
  // END ATOMIC
  
  // ** Update the UI list **
  if (not list_control->empty ())
  {
    // Add all the new data.
    for (control::item_iterator l_cursor (list_control->begin ());
         l_cursor != list_control->end ();
         ++l_cursor)
      {
        set_row (m_model->append (), l_cursor);
      }
    // Select the first item in the list
    cursor = list_control->begin ();
    m_list->get_selection ()->select (find_row (cursor));
    // Should now be in state_two
    ENSURE (is_state_four (), "When setting a parent with children we should be in state four.");
  }
  else
  {
    // Should now be in state_two
    ENSURE (is_state_two (), "When setting a parent without children we should be in state two.");
  }
  
  // Update the rest of the view (update() ENSUREs not is_changed())
  update ();
  
  // Test we have a valid object.
  is_valid ();
}

//Update the display to view the current data. This transfers data from the control object into UI objects.
//
//(The virtual part of the operation is in virt_update, which should be overridden in derived classes.)
//\post not is_changed ().

/**
 * Update the display to view the current data. This transfers data from the control object into UI objects.
 * 
 * (The virtual part of the operation is in virt_update, which should be overridden in derived classes.)
 * \post not is_changed ().
 */
void itemlist_vw::update() {
  // PRE
  is_valid ();
  // DO
  // Set sensitivity
  get_rep ()->set_sensitive (not is_state_one ());
  m_button->set_sensitive (not is_state_one ());
  m_list->set_sensitive (is_state_three () or is_state_four ());
  // Update rest of view.
  virt_update ();
  // END
  ENSURE (false == is_changed (), "After update a view should have no changes");
}


} // namespace expt
