// ------------------------------------------------------------
// class: test_cfm_report
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/report_test/test_cfm_report.hpp"
#include "data_scheme/program.hpp"
#include "report/project_cfm_report.hpp"

// Manual include
#include "utility/program_options.hpp"
#include "utility/filesystem.hpp"
// --
namespace report {

/**
 * The Singleton test object.
 */
test_cfm_report test_cfm_report::s_exemplar;

/**
 * Override in derived classes to perform unit test.
 */
void test_cfm_report::main_test() const 
{
  // Read database
  data_scheme::program l_program;
  l_program.connect (m_database);
  if (l_program.begin_project () != l_program.end_project ())
  {
    // Generate report on first project
    report::project_cfm_report l_reporter;
    std::ostringstream l_os;
    l_reporter (*(l_program.begin_project ()), l_os);
    const std::string l_buffer (l_os.str ());
    log () << COMM << "Writen file is:\n----\n" << l_buffer << "\n----\n";
    if (not m_test_file.empty ())
    {
      bool_test (utility::filesystem::compare (m_test_file, l_buffer.data (), l_buffer.size ()), "original file matches write buffer");
    }
  }

}

/**
 * This method is called by the manager to allow test types to 
 * register program options.  The default implementation does
 * nothing.
 */
void test_cfm_report::register_options() 
{
  utility::program_options::helper ().public_options ().add_options ()
    ("database", boost::program_options::value< std::string >(&m_database), "XML database file")
    ("test-output", boost::program_options::value< std::string >(&m_test_file), "expected XML output file");
  

}


} // namespace report
