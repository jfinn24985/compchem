// ------------------------------------------------------------
// class: ui_traits_test
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/expt/ui_traits_test.hpp"

namespace expt {

//This function tests if a date can be read to and from a UI string to return the same value.

/**
 * This function tests if a date can be read to and from a UI string to return the same value.
 */
void ui_traits_test::test_dates() {
  // LOCAL
  boost::gregorian::date l_now (boost::gregorian::day_clock::local_day())
    , l_when;
  ui_traits::string_t l_date_rep;
  // DO
  ALWAYS (not l_now.is_not_a_date (), "Require start date to be valid.");
  
  // Get date
  l_date_rep = ui_traits::date_to_text (l_now);
  
  // Print date
  std::cout << "Today is [" << l_date_rep << "]" << std::endl;
  
  // Get date back
  l_when = ui_traits::text_to_date (l_date_rep);
  
  // Ask for dates
  do
  {
    std::cout << "Please enter a date string (^d for exit):";
    std::cin >> l_date_rep;
    if (l_date_rep.empty ())
      {
        std::cout << std::endl;
        break;
      }
    l_when = ui_traits::text_to_date (l_date_rep);
    if (l_when.is_not_a_date ())
      {
        std::cout << "Entered date is not valid." << std::endl;
      }
    else
      {
        std::cout << "Date is [" << ui_traits::date_to_text (l_when) << "]" << std::endl;
      }
  } while (true);
  
  // Compare
  ALWAYS (l_now == l_when, "Two dates are not the same after converting to text");
  
  // DONE
}

//Test the "ask_query" method

/**
 * Test the "ask_query" method
 */
void ui_traits_test::test_ask_query() {
  // LOCAL
  ui_traits::ask_info l_info_one ("This is the query text"
  			    , "This is a help text for the query"
  			    , "Yes Text"
  			    , "No Text"
  			    , "Cancel Text");
  ui_traits::ask_info l_info_two ("This is a simple query with 'OK' button");
  unsigned int l_result;
  std::string l_test_me ("Hello World");
  // DO
  std::cout << "You should see a dialog with the following information:" << std::endl;
  show_ask_info (std::cout, l_info_one);
  std::cout << "Original text: " << l_test_me << std::endl;
  l_result = ui_traits::ask_query (l_info_one, l_test_me);
  
  if (ui_traits::NO == l_result)
  {
    std::cout << "No button pressed" << std::endl;
    std::cout << "Editted text: " << l_test_me << std::endl; 
  }
  else if (ui_traits::YES == l_result)
  {
    std::cout << "Yes button pressed" << std::endl;
    std::cout << "Editted text: " << l_test_me << std::endl; 
  }
  else if (ui_traits::CANCEL == l_result)
  {
    std::cout << "Cancel button pressed" << std::endl;
    std::cout << "Editted text: " << l_test_me << std::endl; 
  }
  else
  {
    std::cout << "Some other exit occurred" << std::endl;
  }
  
  // Test info only dialog
  std::cout << "You should see a simple dialog with the following information:" << std::endl;
  show_ask_info (std::cout, l_info_two);
  l_result = ui_traits::ask_query (l_info_two, l_test_me);
  
  if (ui_traits::NO == l_result)
  {
    std::cout << "No button pressed" << std::endl;
    std::cout << "Editted text: " << l_test_me << std::endl; 
  }
  else if (ui_traits::YES == l_result)
  {
    std::cout << "Yes button pressed" << std::endl;
    std::cout << "Editted text: " << l_test_me << std::endl; 
  }
  else if (ui_traits::CANCEL == l_result)
  {
    std::cout << "Cancel button pressed" << std::endl;
    std::cout << "Editted text: " << l_test_me << std::endl; 
  }
  else
  {
    std::cout << "Some other exit occurred" << std::endl;
  }
  
}

//Test the "ask_user" method

/**
 * Test the "ask_user" method
 */
void ui_traits_test::test_ask_url() {
  // LOCAL
  const ui_traits::ask_info l_info_one ("This is the file/URL title text"
  				      , "This is the file/URL help text"
  				      , "Yes Text"
  				      , "No Text"
  				      , "Cancel Text");
  const ui_traits::ask_info l_info_two ("This is a simple open-file/URL text");
  const ui_traits::ask_info l_info_three ("This is a simple save-file/URL text");
  unsigned int l_result;
  std::string l_filename ("hello");
  // DO
  std::cout << "You should see a dialog with the following information:" << std::endl;
  show_ask_info (std::cout, l_info_one);
  std::cout << "This is the original filename: \"" << l_filename << "\"" << std::endl;
  l_result = ui_traits::ask_url (l_info_one, l_filename, true);
  
  if (ui_traits::NO == l_result)
  {
    std::cout << "No button pressed" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  else if (ui_traits::YES == l_result)
  {
    std::cout << "Yes button pressed" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  else if (ui_traits::CANCEL == l_result)
  {
    std::cout << "Cancel button pressed" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  else
  {
    std::cout << "Some other exit occurred" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  
  // Test default buttons dialog
  std::cout << "You should see a simple dialog with the following information:" << std::endl;
  show_ask_info (std::cout, l_info_two);
  l_result = ui_traits::ask_url (l_info_two, l_filename, true);
  
  
  if (ui_traits::NO == l_result)
  {
    std::cout << "No button pressed" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  else if (ui_traits::YES == l_result)
  {
    std::cout << "Yes button pressed" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  else if (ui_traits::CANCEL == l_result)
  {
    std::cout << "Cancel button pressed" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  else
  {
    std::cout << "Some other exit occurred" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  
  // Test default buttons dialog
  std::cout << "You should see a simple dialog with the following information:" << std::endl;
  show_ask_info (std::cout, l_info_three);
  l_result = ui_traits::ask_url (l_info_three, l_filename, false);
  
  if (ui_traits::NO == l_result)
  {
    std::cout << "No button pressed" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  else if (ui_traits::YES == l_result)
  {
    std::cout << "Yes button pressed" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  else if (ui_traits::CANCEL == l_result)
  {
    std::cout << "Cancel button pressed" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  else
  {
    std::cout << "Some other exit occurred" << std::endl;
    std::cout << "This is the final filename: \"" << l_filename << "\"" << std::endl;
  }
  
}

//Test the "ask_user" method

/**
 * Test the "ask_user" method
 */
void ui_traits_test::test_ask_user() {
  // LOCAL
  ui_traits::ask_info l_info_one ("This is the main text"
  			    , "This is the help text"
  			    , "Yes Text"
  			    , "No Text"
  			    , "Cancel Text");
  ui_traits::ask_info l_info_two ("This is a simple information only text");
  unsigned int l_result;
  // DO
  std::cout << "You should see a dialog with the following information:" << std::endl;
  show_ask_info (std::cout, l_info_one);
  l_result = ui_traits::ask_user (l_info_one);
  
  if (ui_traits::NO == l_result)
    std::cout << "No button pressed" << std::endl;
  else if (ui_traits::YES == l_result)
    std::cout << "Yes button pressed" << std::endl;
  else if (ui_traits::CANCEL == l_result)
    std::cout << "Cancel button pressed" << std::endl;
  else
    std::cout << "Some other exit occurred" << std::endl;
  
  // Test info only dialog
  std::cout << "You should see a simple dialog with the following information:" << std::endl;
  show_ask_info (std::cout, l_info_two);
  l_result = ui_traits::ask_user (l_info_two);
  
  if (ui_traits::NO == l_result)
    std::cout << "No button pressed" << std::endl;
  else if (ui_traits::YES == l_result)
    std::cout << "Yes button pressed" << std::endl;
  else if (ui_traits::CANCEL == l_result)
    std::cout << "Cancel button pressed" << std::endl;
  else
    std::cout << "Some other exit occurred" << std::endl;
  
}

int ui_traits_test::main(int argc, char * * argv) {
  Gtk::Main kit(argc, argv);
  
  test_dates ();
  test_ask_user ();
  test_ask_query ();
  test_ask_url ();
  // TestWindow window;
  // Gtk::Main::run(window); //Shows the window and returns when it is closed.
  
  return 0;
}

//Pretty-print the content of a_info on a_os

/**
 * Pretty-print the content of a_info on a_os
 */
void ui_traits_test::show_ask_info(std::ostream & a_os, const ui_traits::ask_info & a_info) {
  // DO
  a_os << "============= ASK_INFO type =============" << std::endl;
  a_os << "       Main message: \"" << a_info.message << "\"" << std::endl;
  a_os << "       Help message: \"" << a_info.help_message << "\"" << std::endl;
  a_os << "   Yes button label: \"" << a_info.yes << "\"" << std::endl;
  a_os << "    No button label: \"" << a_info.no << "\"" << std::endl;
  a_os << "Cancel button label: \"" << a_info.cancel << "\"" << std::endl;
  a_os << "=========================================" << std::endl;
}


} // namespace expt
