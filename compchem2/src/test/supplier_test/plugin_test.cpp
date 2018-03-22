// ------------------------------------------------------------
// class: plugin_test
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/supplier_test/plugin_test.hpp"
#include "supplier/filter_manager.hpp"

// Manual includes
// #include <fstream>
// #include <boost/function.hpp>
#include <boost/filesystem/operations.hpp>
// #include <boost/mem_fn.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
// Entry point function
extern "C" {
int main (int argc, char** argv) {
supplier::plugin_test l_app (argc, argv);
return l_app.result ();}}

namespace supplier {

/**
 * Call back to add to m_manager to get notification when an error occurs.
 */
void plugin_test::error_method(std::string a_msg, int a_id) {
  std::cout << "In Plugin Test Error Handler.\n";
  if (0 != a_id) {
  std::cout << "Error [ID = " << a_id << "]: " << a_msg << "\n"; }
  else {
  std::cout << "Error: " << a_msg << "\n"; }
  m_result = a_id;
  m_error = true;
}

/**
 * Run the application.  This should be run after setting up
 * the m_conf_file and m_filenames.
 * 
 * \pre (not m_conf_file.empty and exists(m_conf_file)) OR nul /= m_manager
 * \pre not m_filenames.empty and for_each(m_filenames, exist)
 */
void plugin_test::main() {
  // PRE
  COMPCHEM_REQUIRE ((not m_conf_file.empty () and boost::filesystem::exists (m_conf_file)) or NULL != m_manager.get (), "Have no list of plugins.");
  COMPCHEM_REQUIRE (not m_filenames.empty (), "Have no target files");
  // LOCAL
  // DO
  if (NULL == m_manager.get ())
  {
    m_manager.reset (new filter_manager (m_conf_file));
  }
  COMPCHEM_CHECK (NULL != m_manager.get (), "Manager should never be nul");
  
  for (std::vector < std::string >::iterator l_source_ptr = m_filenames.begin ();
      l_source_ptr != m_filenames.end ();
      ++l_source_ptr)
  {
    std::cout << title () << ": Source file-name = " << *l_source_ptr << std::endl;
    COMPCHEM_REQUIRE (boost::filesystem::exists (*l_source_ptr), "Target file does not exist");
  
    m_manager->prepare (*l_source_ptr);
  
    // Test have input data
    if (not m_manager->have_file ())
    {
      m_result = 1;
      std::cerr << title () << ": " << *l_source_ptr << " is not recognised source type\n";
    }
    else if (not m_manager->have_filters ())
    {
      m_result = 1;
      std::cerr << title () << ": No valid filters found for source\n";
    }
    else if (m_manager->is_error ())
    {
      m_result = 1;
      std::cerr << title () << ": An error occured while initialising filter system\n";
    }
    else
    {
      std::cout << title () << ": Filter manager has prepared source file." << std::endl;
      // We can now read the entire "gaussian" file and gather the results.
      purpletree::dom::document l_output (data_scheme::document_type::new_document (purpletree::dom::set_utf8 ("memo")));
      data_scheme::memo l_top (l_output.document_element ());
  
      m_manager->process (l_top);
      std::cout << title () << ": ---- Start filter results  ---- \n";
      l_output.implementation ().serialise (l_output, std::cout);
      std::cout << title () << ": ---- End filter results  ---- \n";
    }
    m_manager->reset ();
  }
}

/**
 * Default Constructor
 */
plugin_test::plugin_test(int argc, char ** argv)
  : m_conf_file ()
  , m_error (false)
  , m_filenames ()
  , m_manager ()
  , m_result () {
    if (process_options (argc, argv)) {
      COMPCHEM_CHECK (not m_filenames.empty (), "No source files.");
      COMPCHEM_CHECK (not m_conf_file.empty (), "No configuration file.");
      COMPCHEM_CHECK (boost::filesystem::exists (m_conf_file), ("No configuration file: " + m_conf_file));
      main (); }}

/**
 * Process the command line options.  Returns true if options
 * where processed successfully.
 */
bool plugin_test::process_options(int argc, char ** argv) {
  // LOCAL
  bool Result (true);
  // The Option Processing system
  boost::program_options::options_description l_desc ("Plugin test options");
  // Add program specific options
  l_desc.add_options () ("help,h", "produce help")
      ("usage,?", "also produce help")
      ("file,f", boost::program_options::value< std::vector< std::string > > (&m_filenames), "gaussian log filename(s) to process [required].")
      ("conf,c", boost::program_options::value< std::string > (&m_conf_file), "configuration file [required]");
  // Set arguments without preceding options to act as if predeced by -file
  boost::program_options::positional_options_description l_pos_args;
  l_pos_args.add("file", -1);
  // Process arguments
  boost::program_options::variables_map l_vm;
  try {
    boost::program_options::store (boost::program_options::command_line_parser (argc, argv).options (l_desc).positional (l_pos_args).run (), l_vm); }
  catch (const std::exception& a_err) { // Assume option parsing error.
    std::cout << "Error: " << a_err.what () << "\n";
    Result = false; }
  boost::program_options::notify (l_vm);
  // Output help if require
  if (not Result or l_vm.count("help") or l_vm.count("usage") or not l_vm.count("file") or not l_vm.count("conf")) {
    std::cout << l_desc << "\n";
    Result = false; }
  return Result;
}


} // namespace supplier
