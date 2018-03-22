// ------------------------------------------------------------
// class: cfm_report
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "report/ccreport/cfm_report.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/program.hpp"
#include "report/project_cfm_report.hpp"
#include "data_scheme/scoped_open.hpp"

namespace ccreport {

/**
 * Override in derived classes to perform unit test.
 */
int cfm_report::main() const 
{
  int Result (0);
  if (m_database.empty ())
  {
    std::cout << "Error: No database defined, please set a default database or supply on the\ncommand line.\n";
  }
  else if (data_scheme::data_scheme_helper::helper ().target_project ().empty ())
  {
    std::cout << "Error: No target project defined, please supply one on the command line.\n";
  }
  else
  {
  // Read database
    data_scheme::program l_program;
    l_program.connect (m_database);
    data_scheme::program::project_iterator l_proj (l_program.find_project (data_scheme::data_scheme_helper::helper ().target_project ()));
    if (l_proj != l_program.end_project ())
    {
      // Generate report on the project
      data_scheme::scoped_open < data_scheme::project > l_local_open (*l_proj);
      report::project_cfm_report l_reporter;
      if (m_output.empty ())
      {
        l_reporter (*l_proj, std::cout);
      }
      else 
      {
        std::ofstream l_out (m_output.c_str ());
        l_reporter (*l_proj, l_out);
      }
    }
    else
    {
      std::cout << "Error: Project \"" << data_scheme::data_scheme_helper::helper ().target_project () << "\" could not be found in the database \"" << m_database << "\"\n";
      Result = 1;
    }
  }
  
  return Result;

}

/**
 * This method is called by the manager to allow test types to 
 * register program options.  The default implementation does
 * nothing.
 */
void cfm_report::register_options() 
{
  utility::program_options::helper ().public_options ().add_options ()
    ("database", boost::program_options::value< std::string >(&m_database), "XML database file")
    ("output,o", boost::program_options::value< std::string >(&m_output),"The output filename.");
  

}


} // namespace ccreport

int main (int argc, char ** argv)
{
  ccreport::cfm_report l_app;
  return l_app.programs::basic_application< ccreport::cfm_report >::main (argc, argv);
}
