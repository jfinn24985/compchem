// ------------------------------------------------------------
// class: pam_test
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/utility_test/pam_test.hpp"

namespace utility {

/**
 * The singleton test object.
 */
authentication_test authentication_test::s_exemplar;

/**
 * Destructor, should remove test case?
 */
authentication_test::~authentication_test() {
}

/**
 * The method that performs tests on the authentication manager exemplar
 */
void authentication_test::main_test() const {
  // local
  try {
    const boost::program_options::variable_map & l_vm (utility::test_manager::get_exemplar ().program_options ());
    if (0 < l_vm.count ("user") and 0 < l_vm.count ("auth")) {
    std::string l_initial_user(l_vm["user"].as< std::string >());
    std::auto_ptr< authentication_manager > l_man;
    
    // do
    l_man = authentication_manager::create_manager (l_initial_user, l_vm["auth"].as< std::string >());
    
    // This will exit on failure
    COMPCHEM_ALWAYS (NULL != l_man.get (), "Unable to create an authentication manager");
    
    // Step One: Is manager valid?
    if (false == l_man->is_valid ()) {
      log () << EXCP << "Authentication manager is invalid: \n";
      log () << EXCP << "Error is: " << l_man->error_string () << std::endl; }
    else {
      // Step Two: Is the user authenticated?
      if (false == l_man->is_authenticated ()) {
        log () << ISNO << "Authentication failed (" << l_man->error_string () << ") "; }
      else {
        log () << ISYS << "Authentication succeeded "; }
      // Step Three: Find out who the authenticated user is.
      log () << WTCH << "for user [" << l_man->get_user () << "]\n"; }
    // Done
    log () << COMM << "Finised authentication.\n"; }
    else {
      log () << EXCP << "Need to set 'auth' and 'user' program options.\n"; }}
  catch (const std::exception & a_err) {
    log () << EXCP << "Caught exception: " << a_err.what (); }
}

/**
 * This method is called by the manager to allow test types to 
 * register program options.  The default implementation does
 * nothing.
 */
void authentication_test::register_options() 
{
  if (NULL == utility::program_options::helper ().public_options ().find_nothrow ("user", true))
  {
    // Add option
    utility::program_options::helper ().public_options ().add_options () ("user", boost::program_options::value< std::string >(), "User to authenticate.");
  }
  if (NULL == utility::program_options::helper ().public_options ().find_nothrow ("auth", true))
  {
    // Add option
    utility::program_options::helper ().public_options ().add_options () ("auth", boost::program_options::value< std::string >()->default_value ("cchem"), "Authentication class for validation.");
  }

}


} // namespace utility
