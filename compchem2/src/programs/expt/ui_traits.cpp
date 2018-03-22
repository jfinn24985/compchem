// ------------------------------------------------------------
// class: ui_traits
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/ui_traits.hpp"

namespace expt {

//Ask the user for a piece of information with initial value given by and returned in a_value.  If you do not specify (or specify an empty string) Yes/No/Cancel buttons titles then the corresponding button is not diplayed. If not titles are set then a default "Ok" button will be used (which returns YES).
//
//Note that the NO value is returned if the dialog is closed by the window manager.
//\post result == YES, NO or CANCEL

/**
 * Ask the user for a piece of information with initial value given by and returned in a_value.  If you do not specify (or specify an empty string) Yes/No/Cancel buttons titles then the corresponding button is not diplayed. If not titles are set then a default "Ok" button will be used (which returns YES).
 * 
 * Note that the NO value is returned if the dialog is closed by the window manager.
 * \post result == YES, NO or CANCEL
 */
unsigned int ui_traits::ask_query(const ui_traits::ask_info & a_message, std::string & a_value)
{
  // Add an entry widget
  Gtk::Entry l_text;
  unsigned int l_mask (0);
  // Create a mask of possible response values.
  if (not a_message.yes.empty ()) l_mask |= YES;
  if (not a_message.no.empty ()) l_mask |= NO;
  if (not a_message.cancel.empty ()) l_mask |= CANCEL;
  // Create a dialog, if we have button texts add no buttons here.
  Gtk::MessageDialog l_diag (a_message.message
  			   , false
  			   , Gtk::MESSAGE_QUESTION
  			   , ((0 == l_mask)?Gtk::BUTTONS_YES_NO:Gtk::BUTTONS_NONE)
  			   , false);
  // If the mask is empty, set to accept 'OK' and 'CLOSE'
  if (0 == l_mask)
  {
    l_mask = (YES | NO);
  }
  // Add help message if present
  if (not a_message.help_message.empty ())
  {
    l_diag.set_secondary_text (a_message.help_message);
  }
  // Add buttons if present
  if (not a_message.yes.empty ())
    l_diag.add_button (a_message.yes, Gtk::RESPONSE_YES);
  if (not a_message.no.empty ())
    l_diag.add_button (a_message.no, Gtk::RESPONSE_NO);
  if (not a_message.cancel.empty ())
    l_diag.add_button (a_message.cancel, Gtk::RESPONSE_CANCEL);
  
  // Add the text box
  l_diag.get_vbox ()->add (l_text);
  l_text.set_text (a_value);
  l_text.show_all ();
  
  // Run dialog and translate response value
  unsigned int Result (response_conv (l_diag.run ()));
  
  // Get the text if response was YES
  if (YES == Result)
  {
    a_value = l_text.get_text ();
  }
  
  // Test response is valid
  ENSURE (0 != (l_mask & Result)
  	, "Unknown or invalid response from dialog.");
  return Result;
}

//Ask the user for a filename or URL.  Use a_message to inform the user the reason for request.  If a_message is an empty string then a default message is displayed.
//
//The a_filename is used to obtain an initial filename.  It is used as the starting location presented to the user.  If it is empty then the current working directory is displayed.  When a filename is selected a_filename set to the value.
//
//TODO: ensure a_filename is a valid path.  If a_filename is an empty string, use the current working directory.
//
//If Yes or Cancel button titles are empty strings then default buttons will be used.  If No button title is empty then no corresponding button will be displayed. 
//
//\param a_is_read  Whether the file will be read or writen to.  When a_is_read is true then the document must already exist.  When false then a non-existant document may be chosen. (Default is false)
//\post result == YES, NO or CANCEL

/**
 * Ask the user for a filename or URL.  Use a_message to inform the user the reason for request.  If a_message is an empty string then a default message is displayed.
 * 
 * The a_filename is used to obtain an initial filename.  It is used as the starting location presented to the user.  If it is empty then the current working directory is displayed.  When a filename is selected a_filename set to the value.
 * 
 * TODO: ensure a_filename is a valid path.  If a_filename is an empty string, use the current working directory.
 * 
 * If Yes or Cancel button titles are empty strings then default buttons will be used.  If No button title is empty then no corresponding button will be displayed. 
 * 
 * \param a_is_read  Whether the file will be read or writen to.  When a_is_read is true then the document must already exist.  When false then a non-existant document may be chosen. (Default is false)
 * \post result == YES, NO or CANCEL
 */
unsigned int ui_traits::ask_url(const ui_traits::ask_info & a_message, std::string & a_filename, bool a_is_read)
{
  // LOCAL
  unsigned int l_mask (0);
  // Create a mask of possible response values.
  if (not a_message.yes.empty ()) l_mask |= YES;
  if (not a_message.no.empty ()) l_mask |= NO;
  if (not a_message.cancel.empty ()) l_mask |= CANCEL;
  // Create a dialog, if we have button texts add no buttons here.
  Gtk::FileChooserDialog l_diag (a_message.message
  			       , (a_is_read)?Gtk::FILE_CHOOSER_ACTION_OPEN:Gtk::FILE_CHOOSER_ACTION_SAVE);
  
  // DO
  // If the mask is empty, set to accept all.
  if (0 == l_mask)
  {
    l_mask = (YES | NO | CANCEL);
  }
  
  // // Add help message if present
  // if (not a_message.help_message.empty ())
  // {
  //   l_diag.set_secondary_text (a_message.help_message);
  // }
  
  // Add buttons if present
  if (not a_message.yes.empty ())
  {
    l_diag.add_button (a_message.yes, Gtk::RESPONSE_YES);
  }
  else
  {
    // Add stock "yes" button
    l_diag.add_button ((a_is_read)?GTK_STOCK_OPEN:GTK_STOCK_SAVE
  		     , Gtk::RESPONSE_YES);
  }
  if (not a_message.no.empty ())
  {
    l_diag.add_button (a_message.no, Gtk::RESPONSE_NO);
  }
  if (not a_message.cancel.empty ())
  {
    l_diag.add_button (a_message.cancel, Gtk::RESPONSE_CANCEL);
  }
  else
  {
    // Add stock "cancel" button
    l_diag.add_button (GTK_STOCK_CANCEL, Gtk::RESPONSE_CANCEL);
  }
  
  // Do some odd things
  gtk_file_chooser_set_local_only (GTK_FILE_CHOOSER(l_diag.gobj ())
  				 , true);
  gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(l_diag.gobj ())
  			       , a_filename.c_str ());
  
  // Run dialog and translate response value
  unsigned int Result (response_conv (l_diag.run ()));
  
  if (YES == Result)
  {
    gchar* l_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(l_diag.gobj ()));
    ALWAYS (l_file != NULL, "No filename selected!");
    a_filename = l_file;
    g_free (l_file);
  }
  // Test response is valid
  ENSURE (0 != (l_mask & Result)
  	, "Unknown or invalid response from dialog.");
  return Result;
}

//Ask the user a yes/no question with option to cancel.  For displaying information messages do not set the Yes/No/Cancel buttons titles then a default "Ok" button will be used and the return value will be relatively meaningless.
//
//Note that the NO value is returned if the dialog is closed by the window manager.
//\post result == YES, NO or CANCEL 

/**
 * Ask the user a yes/no question with option to cancel.  For displaying information messages do not set the Yes/No/Cancel buttons titles then a default "Ok" button will be used and the return value will be relatively meaningless.
 * 
 * Note that the NO value is returned if the dialog is closed by the window manager.
 * \post result == YES, NO or CANCEL 
 */
unsigned int ui_traits::ask_user(const ui_traits::ask_info & a_message)
{
  // LOCAL
  unsigned int l_mask (0);
  // Create a mask of possible response values.
  if (not a_message.yes.empty ()) l_mask |= YES;
  if (not a_message.no.empty ()) l_mask |= NO;
  if (not a_message.cancel.empty ()) l_mask |= CANCEL;
  // Create a dialog, if we have button texts add no buttons here.
  Gtk::MessageDialog l_diag (a_message.message
  			   , false
  			   , Gtk::MESSAGE_INFO
  			   , ((0 == l_mask)?Gtk::BUTTONS_OK:Gtk::BUTTONS_NONE)
  			   , false);
  // If the mask is empty, set to accept 'OK' and 'CLOSE'
  if (0 == l_mask)
  {
    l_mask = (YES | NO);
  }
  // Add help message if present
  if (not a_message.help_message.empty ())
  {
    l_diag.set_secondary_text (a_message.help_message);
  }
  // Add buttons if present
  if (not a_message.yes.empty ())
    l_diag.add_button (a_message.yes, Gtk::RESPONSE_YES);
  if (not a_message.no.empty ())
    l_diag.add_button (a_message.no, Gtk::RESPONSE_NO);
  if (not a_message.cancel.empty ())
    l_diag.add_button (a_message.cancel, Gtk::RESPONSE_CANCEL);
  // Run dialog and translate response value
  unsigned int Result (response_conv (l_diag.run ()));
  // Test response is valid
  ENSURE (0 != (l_mask & Result)
  	, "Unknown or invalid response from dialog.");
  return Result;
}

//Convert a date into a UI string.

/**
 * Convert a date into a UI string.
 */
base_view::string_t ui_traits::date_to_text(const boost::gregorian::date & a_date)
{
  // LOCAL
  Glib::ustring Result;
  
  // DO
  // Get start date as a string
  std::stringstream l_date_ss;
  l_date_ss << a_date;
  // Convert to date.
  l_date_ss >> Result;
  // DONE
  return Result;
}

//Find or create a UI object based on a_key.  The UI object will be created relative to a_parent.
//
//\pre a_key.empty () /= true
//\pre a_parent /= nul

/**
 * Find or create a UI object based on a_key.  The UI object will be created relative to a_parent.
 * 
 * \pre a_key.empty () /= true
 * \pre a_parent /= nul
 */
base_view::window_t ui_traits::find_representation(const base_view::string_t & a_key, base_view::const_window_t a_parent)
{
  // a_key
  // a_parent
  REQUIRE (a_parent != NULL, "Require a valid parent window.");
  REQUIRE (not a_key.empty (), "Require a non-empty key.");
  CHECK (0 != S_xml_map.count (a_parent->get_toplevel ())
         , "Given window does not have an associated XML document.");
  // DO
  static Glib::RefPtr<Gnome::Glade::Xml> l_doc (S_xml_map[a_parent->get_toplevel ()]);
  CHECK (l_doc, "Need a valid XML document.");
  Gtk::Widget* Result (l_doc->get_widget (a_key));
  ENSURE (Result != NULL, ("Cannot find widget using the given key [" + std::string(a_key) + "]").c_str ());
  return Result;
}

//Instantiate UI library and load UI from a_url.  Instantiate window located using a_key as the top-level window.  Return the window.
//
//\pre a_key.empty () /= true amd a_url.empty ()
//
//\post Result /= nul

/**
 * Instantiate UI library and load UI from a_url.  Instantiate window located using a_key as the top-level window.  Return the window.
 * 
 * \pre a_key.empty () /= true amd a_url.empty ()
 * 
 * \post Result /= nul
 */
base_view::window_t ui_traits::find_representation(const base_view::string_t & a_key, const base_view::string_t & a_url)
{
  // a_key
  // a_url
  Glib::RefPtr<Gnome::Glade::Xml> l_doc (Gnome::Glade::Xml::create (a_url));
  ALWAYS (l_doc, "Cannot open UI definition file.");
  // Get widget.
  Gtk::Widget* Result (l_doc->get_widget (a_key));
  S_xml_map[Result->get_toplevel ()] = l_doc;
  ENSURE (Result != NULL, "Cannot find widget using the given key.");
  return Result;
}

//This converts a Gtk::Response value into Yes/No/Cancel.
//
//\post Result == YES or NO or CANCEL

/**
 * This converts a Gtk::Response value into Yes/No/Cancel.
 * 
 * \post Result == YES or NO or CANCEL
 */
unsigned int ui_traits::response_conv(int a_response)
{
  // LOCAL
  unsigned int Result (0);
  // DO
  switch (a_response)
  {
   case Gtk::RESPONSE_YES:
   case Gtk::RESPONSE_ACCEPT:
   case Gtk::RESPONSE_OK:
   case Gtk::RESPONSE_APPLY:
     Result = ui_traits::YES;
     break;
   case Gtk::RESPONSE_NO:
   case Gtk::RESPONSE_NONE:
   case Gtk::RESPONSE_REJECT:
   case Gtk::RESPONSE_DELETE_EVENT:
     Result = ui_traits::NO;
     break;
   case Gtk::RESPONSE_CANCEL:
   case Gtk::RESPONSE_CLOSE:
     Result = ui_traits::CANCEL;
     break;
   default:
     ALWAYS (false and a_response, "Unknown response type");
     break;
  }
  ENSURE (YES == Result or NO == Result or CANCEL == Result
  	, "Unknown response from dialog.");
  return Result;
}

//Convert a UI string into a date.

/**
 * Convert a UI string into a date.
 */
boost::gregorian::date ui_traits::text_to_date(const base_view::string_t & a_str_date)
{
  // LOCAL
  boost::gregorian::date Result (boost::gregorian::not_a_date_time);
  
  // DO
  // Get start date as a string
  std::stringstream l_date_ss;
  l_date_ss << a_str_date;
  // Convert to date.
  l_date_ss >> Result;
  // DONE
  return Result;
}

const unsigned int ui_traits::NO(1);

const unsigned int ui_traits::CANCEL(4);

const unsigned int ui_traits::YES(2);


} // namespace expt
