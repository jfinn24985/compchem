// ------------------------------------------------------------
// class: journal_vw
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/journal_vw.hpp"

namespace expt {

//[UI callback] Optionally closes the currently viewed journal (and any associated views).  This will cause this object to be deconstructed.  If there are unsaved changes then the user is queried and the user may elect to cancel the close process.
//
//\pre is_open ()

/**
 * [UI callback] Optionally closes the currently viewed journal (and any associated views).  This will cause this object to be deconstructed.  If there are unsaved changes then the user is queried and the user may elect to cancel the close process.
 * 
 * \pre is_open ()
 */
void journal_vw::close_document() {
  // LOCAL
  bool l_do_delete (true);
  // PRE
  REQUIRE(is_open (), "Cannot close a non-existent document");
  // DO
  // Skip save if document is not dirty.
  if (journal->is_dirty ())
  {
    unsigned int l_ask (ui_traits::ask_user (ui_traits::ask_info ("Document has unsaved modifications, save before closing?", "Selecting 'Discard' will lose any unsaved changes.", "Save", "Discard", "Cancel")));
    if (ui_traits::YES == l_ask)
      {
        // Start save document use case.
        save_document ();
        if (journal->is_dirty ())
  	{
  	  // Save failed, aborting close document
  	  ui_traits::ask_user (ui_traits::ask_info ("Failed to save document so will not close the document"));
  	  // Stop deletion
  	  l_do_delete = false;
  	} 
      }
    else if (ui_traits::CANCEL == l_ask)
      {
        // Stop deletion
        l_do_delete = false;
      }
  }
  // Test if we should cancel delete.
  if (true == l_do_delete)
  {
    // Disconnect views
    page_view.clear_parent_node ();
  
    // close journal
    journal.reset ();
    // POST
    CHECK (not is_open (), "Resetting the journal should have closed the open document");
  
    // Send signal
    open_document_signal.emit (false);  
  }
}

//[UI callback] This function attempts to exit the application.  This function will test to see if there are any unsaved changes.  If there are unsaved changes and then a_unstoppable is true a back-up version of the journal will be saved otherwise the user will be queried for the save action.  This method should never return.

/**
 * [UI callback] This function attempts to exit the application.  This function will test to see if there are any unsaved changes.  If there are unsaved changes and then a_unstoppable is true a back-up version of the journal will be saved otherwise the user will be queried for the save action.  This method should never return.
 */
void journal_vw::exit() {
  // PRE
  is_valid ();
  // DO
  // Test if we have an open & dirty document.
  if (is_open ())
  {
    if (journal->is_dirty ())
      {
        // Attempt to close document nicely.
        close_document ();
      }
    else
      {
        // Disconnect views
        page_view.clear_parent_node ();
  
        // close journal
        journal.reset ();
  
        // Send signal
        open_document_signal.emit (false);
      }
  }
  // Quit the UI event loop		
  Gtk::Main::quit ();
}

//Test invariant conditions

/**
 * Test invariant conditions
 */
void journal_vw::is_valid() const {
  INVARIANT (is_open () != page_view.is_state_one (), "Journal and page view must have active state together.");
}

//This function attempts to exit the application.  This function will test to see if there are any unsaved changes.  If there are unsaved changes and then a_unstoppable is true a back-up version of the journal will be saved otherwise the user will be queried for the save action.  This method should never return.

/**
 * This function attempts to exit the application.  This function will test to see if there are any unsaved changes.  If there are unsaved changes and then a_unstoppable is true a back-up version of the journal will be saved otherwise the user will be queried for the save action.  This method should never return.
 */
void journal_vw::no_stop_exit() {
  // PRE
  is_valid ();
  // DO
  // Test if we have an open & dirty document.
  if (is_open ())
  {
    if (journal->is_dirty ())
      {
        if (journal->has_filename ())
  	{
  	  journal->set_filename (journal->get_filename () + "." + get_backup_suffix ());
  	}
        else
  	{
  	  journal->set_filename ("./EXPT.SAVE");
  	}
        journal->serialise ();
      }
    // Delete journal.
    page_view.clear_parent_node ();
  
    // close journal
    journal.reset ();
    // POST
    CHECK (not is_open (), "Function should have closed the open document");
  
    // Send signal
    open_document_signal.emit (false);
  }
  // Quit the UI event loop.			
  Gtk::Main::quit ();
}

//[UI callback] Open and display an existing journal document.  This will close a document if one is open.  The user is asked several questions which can lead to cancelling the process, so there may not be an open document at the end.
//
//The user requests a journal stored in a permanent storage be loaded into the current application.  This version looks at the case when only a single journal can be opened at a time. If is_open () returns true then "Close Journal" use-case is called before ask_url().

/**
 * [UI callback] Open and display an existing journal document.  This will close a document if one is open.  The user is asked several questions which can lead to cancelling the process, so there may not be an open document at the end.
 * 
 * The user requests a journal stored in a permanent storage be loaded into the current application.  This version looks at the case when only a single journal can be opened at a time. If is_open () returns true then "Close Journal" use-case is called before ask_url().
 */
void journal_vw::open_document() {
  // LOCAL
  const ui_traits::ask_info l_message ("Open Journal"
  				     , "*.xml"
  				     , "Open"
  				     , std::string()
  				     , "Cancel");
  std::string l_filename (boost::filesystem::initial_path ().string () + "/.");
  // PRE
  is_valid ();
  // DO
  
  // Test to see if there is already a journal open.
  if (is_open ())
  {
    // Attempt to close the document. 
    close_document ();
  }
  
  // If the document is not closed then no new document will be opened.
  // Otherwise request a filename and open the journal document on YES.
  if (not is_open ()
      && ui_traits::YES == ui_traits::ask_url (l_message
  					     , l_filename
  					     , true))
  {
    if (control::journal_ctl::is_journal_document(l_filename))
      {
        journal = control::journal_ctl::open (l_filename);
        // Successfully opened document
        // Build connection between view and control object
        CHECK (page_view.is_state_one ()
  	     , "Can only open document when sub-views are inactive.");
        // Set the page view parent node.
        page_view.set_parent_node (journal->get_document_element ());
        
        // Connect journal view dirty_doc signal to journal control signal.
        journal->get_is_dirty_signal ().connect (sigc::mem_fun (dirty_document_signal
  							      , &sigc::signal1< void, bool >::emit));
        
        // Send signal to indicate a document is now open and clean.
        open_document_signal.emit (true);
        dirty_document_signal.emit (false);
      }
    else
      {
        // Failed to open document
        const std::string l_help ("Unable to open document [" + l_filename + "].  Either there is no file of this name or the document is not of the right type.");
        const ui_traits::ask_info l_message ("Failed to open document"
  					   , l_help);
        ui_traits::ask_user (l_message);
      }
  }
  // POST
  is_valid ();
}

//[UI callback] Open and display an newly created journal document. This will close a document if one is open.  The user is asked several questions if a document is closed, so the document may be the existing document.
//
//\post is_open ()

/**
 * [UI callback] Open and display an newly created journal document. This will close a document if one is open.  The user is asked several questions if a document is closed, so the document may be the existing document.
 * 
 * \post is_open ()
 */
void journal_vw::open_new_document() {
  // PRE
  is_valid ();
  // DO
  
  // Test to see if there is already a journal open.
  if (is_open ())
  {
    // Attempt to close the document.
    close_document ();
  }
  
  // If the document is not closed then no new document will be
  // opened. Otherwise create the journal document.
  if (not is_open ())
  {
    CHECK (page_view.is_state_one ()
  	 , "Can only open a document when sub-views are inactive.");
    journal = control::journal_ctl::create ();   
    CHECK (is_open (), "Unable to create a new document.");
  
    // Set the page view parent node.
    page_view.set_parent_node (journal->get_document_element ());
  
    // Connect journal view dirty_doc signal to journal control signal.
    journal->get_is_dirty_signal ().connect (sigc::mem_fun (dirty_document_signal
  							  , &sigc::signal1< void, bool >::emit));
    
    // Send signal to indicate a document is now open and clean.
    open_document_signal.emit (true);
    dirty_document_signal.emit (true);
  }
  // POST
  is_valid ();
  ENSURE (is_open (), "Should have old or new document at end of open_new_document");
}

//Main constructor.  

/**
 * Main constructor.  
 */
journal_vw::journal_vw()
: base_view (JOURNAL_VIEW_KEY, UI_SOURCE_URL)
, journal ()
, page_view (*this)
, open_document_signal ()
, dirty_document_signal ()
{
// PRE
is_valid ();
// LOCAL
Gtk::MenuItem* l_item (NULL);
Gtk::Widget* l_filemenu (NULL);

l_filemenu = ui_traits::find_representation ("file_menu"
					     , get_rep ());
CHECK (NULL != l_filemenu, "Could not find 'file' menu");
// // Bind menu item to methods
// // Get File/Open ...
l_item = dynamic_cast< Gtk::MenuItem* > (ui_traits::find_representation ("menu_open", get_rep ()));
CHECK (NULL != l_item, "Could not find 'file -> open' menu item");
l_item->signal_activate ().connect (sigc::mem_fun (this, &journal_vw::open_document));

// // Get File/New
l_item = dynamic_cast< Gtk::MenuItem* > (ui_traits::find_representation ("menu_new", get_rep ()));
CHECK (NULL != l_item, "Could not find 'file -> new' menu item");
l_item->signal_activate ().connect (sigc::mem_fun (this, &journal_vw::open_new_document));

// // Get File/Save
l_item = dynamic_cast< Gtk::MenuItem* > (ui_traits::find_representation ("menu_save", get_rep ()));
CHECK (NULL != l_item, "Could not find 'file -> save' menu item");
l_item->signal_activate ().connect (sigc::mem_fun (this, &journal_vw::save_document));
dirty_document_signal.connect (sigc::bind< 0 > (sigc::mem_fun (this, &journal_vw::save_sensitivity_test), base_view::window_t (l_item)));
l_item->set_sensitive (false);

// // Get File/Save As
l_item = dynamic_cast< Gtk::MenuItem* > (ui_traits::find_representation ("menu_save_as", get_rep ()));
CHECK (NULL != l_item, "Could not find 'file -> save as' menu item");
l_item->signal_activate ().connect (sigc::mem_fun (this, &journal_vw::save_as_document));
dirty_document_signal.connect (sigc::mem_fun (l_item, &Gtk::Widget::set_sensitive));
l_item->set_sensitive (false);

// // Get File/Close
l_item = dynamic_cast< Gtk::MenuItem* > (ui_traits::find_representation ("menu_close", get_rep ()));
CHECK (NULL != l_item, "Could not find 'file -> close' menu item");
l_item->signal_activate ().connect (sigc::mem_fun (this, &journal_vw::close_document));
open_document_signal.connect (sigc::mem_fun (l_item, &Gtk::Widget::set_sensitive));
l_item->set_sensitive (false);

// // Get File/Quit
l_item = dynamic_cast< Gtk::MenuItem* > (ui_traits::find_representation ("menu_quit", get_rep ()));
CHECK (NULL != l_item, "Could not find 'file -> quit' menu item");
l_item->signal_activate ().connect (sigc::mem_fun (this, &journal_vw::exit));
}

journal_vw::~journal_vw(){
  // if we have an open document try to close it
  if (is_open ())
  {
    no_stop_exit ();
  }
}

/**
 * Not defined, added to stop copying.
 */
journal_vw::journal_vw(const journal_vw & source){
}

/**
 * Not defined, added to stop copying.
 */
journal_vw & journal_vw::operator =(const journal_vw & source) {
}

//[UI callback] Save the journal document.  This may fail to save the document.

/**
 * [UI callback] Save the journal document.  This may fail to save the document.
 */
void journal_vw::save_as_document() {
  // LOCAL
  std::string l_filename;
  // DO
  // Request to save journal under a new filename
  if (ui_traits::YES == ui_traits::ask_url (ui_traits::ask_info ("Filename to save journal"), l_filename))
  {
    do 
      {
        // TEST to see if filename is valid
        std::cerr << "NEED TEST to see if filename [" << l_filename << "] is valid" << std::endl;
        journal->set_filename (l_filename);
      }
    while (not journal->serialise ()
  	 and ui_traits::YES == ui_traits::ask_url (ui_traits::ask_info ("Unable to save using filename \"" + l_filename + "\", try a different file name?"), l_filename));
  }
}

//[UI callback] Save the journal document.  This may fail.

/**
 * [UI callback] Save the journal document.  This may fail.
 */
void journal_vw::save_document() {
  // LOCAL
  // DO
  // Test if journal has a filename assigned.
  if (not journal->has_filename ())
  {
    save_as_document ();
  }
  else
  {
    if (not journal->serialise ())
      {
        if (ui_traits::YES == ui_traits::ask_user (ui_traits::ask_info ("Unable to save document, attempt to save under a different file name?")))
  	{
  	   save_as_document ();
  	}
      }
  }
}

//[impl] Used to set the senstivity of a "save" widget to indicate when a document (is_open and) is_dirty and has_filename.
//
//\pre is_open ()
//\pre a_widget /= nul

/**
 * [impl] Used to set the senstivity of a "save" widget to indicate when a document (is_open and) is_dirty and has_filename.
 * 
 * \pre is_open ()
 * \pre a_widget /= nul
 */
void journal_vw::save_sensitivity_test(base_view::window_t & a_item, bool a_is_dirty) {
  REQUIRE (is_open (), "Cannot query a closed journal document.");
  REQUIRE (NULL != a_item, "Cannot set property of a null window object.");
  is_valid ();
  //   std::cout << "Called set_doc_sensitivity for "
  // 	    << std::string (a_item->get_name())
  // 	    << " with "
  // 	    << ((is_open ())? "an open":"no")
  // 	    << " document." << std::endl;
  a_item->set_sensitive (a_is_dirty
  		       and journal->has_filename ());
}

const base_view::string_t journal_vw::JOURNAL_VIEW_KEY("journal_window");

const base_view::string_t journal_vw::UI_SOURCE_URL("expt.glade");

std::string journal_vw::backup_suffix;

void journal_vw::set_backup_suffix(const std::string value)
{
  backup_suffix = value;
}


} // namespace expt
