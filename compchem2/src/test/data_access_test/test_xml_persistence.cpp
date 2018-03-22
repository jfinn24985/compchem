// ------------------------------------------------------------
// class: test_xml_persistence
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/data_access_test/test_xml_persistence.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/program.hpp"
#include "utility/program_options.hpp"
#include "data_scheme/project.hpp"
#include "data_access/xml_persistence.hpp"
#include "data_access/xml_write_visitor.hpp"
#include "data_access/xml_access_helper.hpp"

// Manual includes in source
#include <boost/bind.hpp>
#include "utility/filesystem.hpp"
// -
namespace data_access {

/**
 * Singleton instance.
 */
test_xml_persistence test_xml_persistence::s_exemplar;

/**
 * Override in derived classes to perform unit test.
 * 
 * Failed tests are indicated by an uncaught exception.
 */
void test_xml_persistence::main_test() const 
{
  // Perform search.
  log () << "Main document path: " << m_base.string () << "\n";
  bool_test (boost::filesystem::exists (m_base), "main document exists.");
  log () << "Sub-document path: " << m_path.string () << "\n";
  bool_test (boost::filesystem::exists (m_path), "sub-document exists.");
  data_scheme::program l_program;
  l_program.connect (m_base.string ());
  // Find memo (in haystack)
  data_scheme::experiment::memo_iterator l_memo;
  bool l_not_found (true);
  for (data_scheme::program::project_iterator l_project (l_program.begin_project ());
       l_not_found and l_project != l_program.end_project ();
       ++l_project)
  {
    for (data_scheme::project::experiment_iterator l_expt (l_project->begin_experiment ());
         l_not_found and l_expt != l_project->end_experiment ();
         ++l_expt)
    {
      l_memo = l_expt->find_memo (m_key);
      l_not_found = (l_memo == l_expt->end_memo ());
    }
  }
  bool_test (not l_not_found, "memo found from key.");
  std::auto_ptr < xml_persistence > l_file (new xml_persistence (*l_memo, m_path));
  bool_test (NULL != l_file.get (), "persistence object created.");
  log () << TTLE << "Base persistence information\n";
  log () << WTCH << "URI path: " << l_file->uri_path () << "\n";
  log () << WTCH << "URI scheme: " << l_file->uri_scheme () << "\n";
  log () << WTCH << "URI: " << l_file->uri () << "\n";
  log () << TTLE << "XML persistence information\n";
  log () << WTCH << "Base path: " << l_file->base ().string () << "\n";
  log () << WTCH << "Full path: " << l_file->full_path ().string () << "\n";
  log () << WTCH << "Path: " << l_file->path ().string () << "\n";
  log () << WTCH << "Header " << l_file->header () << "\n";
  log () << TTLE << "Reading XML file...\n";
  if (l_memo->is_src ())
  {
    if (not l_memo->is_open ()) l_memo->read ();
  }
  else
  {
    l_memo->make_src (l_file->uri_path ());
  }
  l_file->read ();
  log () << TTLE << "Results after reading XML file.\n";
  log () << TTLE << "Base persistence information\n";
  log () << WTCH << "URI path: " << l_file->uri_path () << "\n";
  log () << WTCH << "URI scheme: " << l_file->uri_scheme () << "\n";
  log () << WTCH << "URI: " << l_file->uri () << "\n";
  log () << TTLE << "XML persistence information\n";
  log () << WTCH << "Base path: " << l_file->base ().string () << "\n";
  log () << WTCH << "Full path: " << l_file->full_path ().string () << "\n";
  log () << WTCH << "Path: " << l_file->path ().string () << "\n";
  log () << WTCH << "Header " << l_file->header () << "\n";
  
  if (not m_test_file.empty ())
  {
    log () << TTLE << "Test the xml_write_visitor class\n";
    std::string l_buffer;
    {
      std::ostringstream l_os;
      data_access::xml_write_visitor l_writer (l_os
                                               , l_memo->serial_name ()
                                               , xml_access_helper::helper ().public_id ()
                                               , xml_access_helper::helper ().system_id ());
      l_os << l_file->header ();
      l_writer.visit_basic_datum (*l_memo);
      l_buffer.assign (l_os.str ());
    } 
    log () << COMM << "Writen file is:\n----\n" << l_buffer << "\n----\n";
    bool_test (utility::filesystem::compare (m_test_file, l_buffer.data (), l_buffer.size ()), "original file matches write buffer");
  }
  // boost::filesystem::path l_path (l_helper.find (l_memo_dfn, *(l_file.get ()), m_key));
  // log () << WTCH << "Found path = \"" << l_path.string () << "\"\n";

}

/**
 * This method is called by the manager to allow test types to 
 * register program options.  The default implementation does
 * nothing.
 */
void test_xml_persistence::register_options() 
{
  utility::program_options::helper ().public_options ().add_options ()
    ("search-path", boost::program_options::value< std::string > ()->notifier (boost::bind (&test_xml_persistence::set_path, this, _1)), "search file name (relative to base)")
    ("search-base", boost::program_options::value< std::string > ()->notifier (boost::bind (&test_xml_persistence::set_base, this, _1)), "search base path")
    ("search-key", boost::program_options::value< std::string > (&(this->m_key)), "search key")
    ("search-test-file", boost::program_options::value< std::string > (&(this->m_test_file)), "copy of XML file to compare our result to.");
  

}

test_xml_persistence::~test_xml_persistence() 
{}


} // namespace data_access
