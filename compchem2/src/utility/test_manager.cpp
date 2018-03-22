// ------------------------------------------------------------
// class: test_manager
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "utility/test_manager.hpp"
#include "utility/program_options.hpp"

// Manual includes
#include <fstream>
#include <boost/filesystem/operations.hpp>
// -
int main (int argc, char** argv) {
  return utility::test_manager::get_exemplar ().main (argc, argv); }

namespace utility {

/**
 * prefix for commentary messages
 */
const std::string base_test::COMM("[--] ");

/**
 * prefix for exception messages
 */
const std::string base_test::EXCP("[EE] ");

/**
 * prefix for messages with info to check or compare manually
 */
const std::string base_test::CHCK("[CC] ");

/**
 * prefix for messages with info to watch
 */
const std::string base_test::WTCH("[RR] ");

/**
 * prefix for section titles
 */
const std::string base_test::TTLE("[TT] ");

/**
 * prefix for messages in program branches when the desired branch is reached
 */
const std::string base_test::IFYS("[##] ");

/**
 * prefix for messages in program branches when the undesired branch is reached
 */
const std::string base_test::IFNO("[%%] ");

/**
 * Get the reference to the logging stream.
 */
std::ostream & base_test::log() throw()

{
  return test_manager::get_exemplar ().log ();
}

/**
 * This method calls a_fn and expects to catch a contract_error of a_level.
 * 
 * Throws logic_errors if function fails to throw an exception and rethrows
 * the contract_error if of the wrong type.
 */
void base_test::monitor_function(unsigned int a_lvl, base_test::funtion_t a_fn, const char *const a_msg)

{
  if (NULL != a_msg)
  {
    log () << WTCH << a_msg << std::endl;
  }
  try
  {
    a_fn();
    const static bool function_throw_test_failed (true);
    COMPCHEM_ALWAYS (function_throw_test_failed, "Test function failed to throw expected error.");
  }
  catch (const utility::contract_error &err)
  {
    const std::string l_what (err.what (), 256);
    if (err.level == a_lvl)
    {
      log () << IFYS << "Caught contract error of correct level [" << err.level << "]\n" << l_what << "\n";
    }
    else
    {
      // Exception is incorrect, rethrow.
      throw;
    }
  }

}

/**
 * The single test_manager instance.
 */
test_manager * test_manager::s_exemplar(NULL);

/**
 * Routine to manage unit tests.
 * 
 * Note: base_test derived classes indicate failure by an uncaught
 * exception.
 */
int test_manager::main(int argc, char ** argv) const {
  // local
  int Result (0);
  // do
  if (empty ())
  {
    log () << "No test routines, nothing to do?.\n";
    Result = 1;
  }
  else
  {
    ///////////////////////////
    // Process the configuration
    if (not utility::program_options::empty ())
    {
      std::string l_log_fname;
      utility::program_options & l_helper (utility::program_options::helper ());
      l_helper.public_options ().add_options () ("log-file", boost::program_options::value< std::string >(&l_log_fname), "Log file for test output");
      std::string l_conffile ("test/canon/");
      l_conffile.append (boost::filesystem::path (argv[0]).leaf ());
      l_conffile.append ("_args.cpp");
      if (boost::filesystem::exists (l_conffile))
      {
        l_helper.config_filename (l_conffile);
      }
      else
      {
        log () << "No configuration file [" << l_conffile << "]\n";
      }
      // Process configuration
      Result = l_helper.process_options (argc, argv, log (), utility::program_options::CCHEM_PARSE_ALL);
      // Free space used for processing program options
      utility::program_options::reset ();
      // Switch to using log file.
      if (not l_log_fname.empty ())
      {
        use_log_file (l_log_fname);
      }
    }
    // Continue if configuration successful.
    if (utility::program_options::CCHEM_PARSE_SUCCESS == Result)
    {
      ////////////////
      // Perform tests
      for (const_iterator l_iter = m_test_routines.begin (); l_iter != m_test_routines.end (); ++l_iter)
      {
        log () << "**** Start unit test [" << ((*l_iter)->title ()) << "] ****\n";
        try
        {
          (*l_iter)->main_test ();
        }
        catch (const std::exception & err)
        {
          log () << base_test::EXCP << "Unhandled std::exception caught by 'main':\n" << err.what () << "\n\n";
          log ().flush ();
          Result = 1;
        }
        catch (...)
        {
          log () << base_test::EXCP << "Unhandled and unknown exception caught by 'main'\n";
          log ().flush ();
          Result = 1;
        }
        log () << "**** Ending unit test [" << (*l_iter)->title () << "] ****\n";
        log ().flush ();
      }
    }
  }
  
  return Result;
}

/**
 * Switch to using this a_fname as file for log messages.
 */
void test_manager::use_log_file(const std::string& a_fname) const 
{
  m_log_stream.reset (new std::ofstream (a_fname.c_str ()));
  m_log = m_log_stream.get ();

}


} // namespace utility
