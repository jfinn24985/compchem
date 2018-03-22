// ------------------------------------------------------------
// class: verify_app
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


// Manual includes
#include <boost/filesystem/operations.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include "data_access/ccverify/verify_app.hpp"
#include "utility/program_options.hpp"
#include "data_scheme/program.hpp"
#include "data_access/ccverify/verify_visitor.hpp"
#include "data_access/xml_access_helper.hpp"
#include "data_scheme/data_scheme_helper.hpp"

namespace data_access {

/**
 * Perform the main operations of the crawler.
 */
int verify_app::main(int argc, char ** argv) 
{
  int Result (utility::program_options::CCHEM_PARSE_SUCCESS); // The return value of this function
  boost::filesystem::initial_path (); // Set initial path
  boost::filesystem::path::default_name_check (&boost::filesystem::native); // Set path name checker
  ////////////////////////////////
  // Processing args etc.
  {
    utility::program_options &l_conf_helper (utility::program_options::helper ());
    l_conf_helper.public_options ().add_options () ("file,f", boost::program_options::value< std::vector< std::string > > (&m_fileset), "data files to process.");
    l_conf_helper.positional_option ("file");
    // Process arguments
    Result = l_conf_helper.process_options (argc, argv, std::cerr, utility::program_options::CCHEM_PARSE_ALL);
  }
  // Free any memory used for option processing.
  utility::program_options::reset ();
  
  //////////////////////////////////
  // Main operation
  if (utility::program_options::CCHEM_PARSE_SUCCESS == Result)
  {
    if (empty ())   // Test we have data to work on
    {
      std::cerr << "No databases to process\n";
      Result = utility::program_options::CCHEM_PARSE_ERROR;
    }
    else   // Perform actions
    {
      // Set to auto-fix any broken links.
      if (not xml_access_helper::helper ().auto_fix ())
      {
        xml_access_helper::helper ().auto_fix (true);
      }
  
      /////////////////////////////////
      // Fix database
      for (const_iterator l_i = begin (); l_i != end (); ++l_i)
      {
        if (not boost::filesystem::exists (*l_i))
        {
          std::cerr << "Cannot find document [" << *l_i <<"], skipping.\n";
        }
        else
        {
          // Create the top-level database object.
          data_scheme::program l_prog;
          l_prog.connect (*l_i);
          // Create the visitor.
          verify_visitor l_visitor;
          // Perform the update
          l_prog.visit (l_visitor);
          // Report state of cross-references.
          l_visitor.report_cross_references (std::cout);
          // Recursively write any changes.
          l_prog.write ();
        }
      }
    }
  }
  if (utility::program_options::CCHEM_PARSE_HELP == Result)
  {
    Result = utility::program_options::CCHEM_PARSE_SUCCESS;
  }
  return Result;

}


} // namespace data_access

int main (int argc, char** argv)
{
  data_access::verify_app l_app;
  return l_app.main (argc, argv);
}

