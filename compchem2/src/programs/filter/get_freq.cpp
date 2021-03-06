// ------------------------------------------------------------
// class: get_freq
// Orig author: Justin Finnerty
// $author:$
// Orig date:
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#define NO_LOG
#include "programs/filter/get_freq.hpp"

// Manual includes
#include <memory>
#include "supplier/filetype/gaussian_file.hpp"
#include "supplier/filter/gaussian_vibration.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "dom.hpp"
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
// -
// Manual decls
extern base_filter_traits plugin_object;
// -
namespace cchem {

namespace programs {

int get_freq::main(int argc, char ** argv) {
  // LOCAL
  const int ON_SUCCESS = 0;
  // const int ON_ERROR = 1;
  const int ON_HELP = 2;
  int Result (ON_SUCCESS); // The return value of this function
  std::auto_ptr< supplier::base_filetype > l_file;
  // The Option Processing system
  boost::program_options::options_description l_desc ("Filter specific options");
  boost::program_options::options_description l_spec ("Spectrum output options");
  boost::program_options::options_description l_all ("All options");
  // Add program specific options
  l_desc.add_options () ("help,h", "produce help")
      ("usage,?", "also produce help")
      ("file,f", boost::program_options::value< std::vector< std::string > > (), "gaussian log filename(s) to process [optional].")
      ("raman,r", boost::program_options::bool_switch (&m_use_raman), "include raman activity as well as infrared intensity.")
      ("Raman,R", boost::program_options::bool_switch (&m_only_raman), "raman activity without infrared intensity.")
      ("spectrum,S", "output a spectrum instead of a peak table");
  // Get Spectrum options
  m_spectrum.register_options (l_spec);
  // Combine all options
  l_all.add (l_desc).add (l_spec);
  boost::program_options::positional_options_description l_pos_args;
  // Set arguments without preceding options to act as if predeced by -file
  l_pos_args.add("file", -1);
  boost::program_options::variables_map l_vm;
  // Process arguments
  try {
    boost::program_options::store (boost::program_options::command_line_parser (argc, argv).options (l_all).positional (l_pos_args).run (), l_vm); }
  catch (const std::exception& a_err) { // Assume option parsing error.
    std::cout << "Error: " << a_err.what () << "\n";
    Result = ON_HELP; }
  boost::program_options::notify(l_vm);
  // Output help if require
  if (ON_HELP == Result or l_vm.count("help") or l_vm.count("usage") or not l_vm.count("file")) {
    std::cout << l_desc << "\n";
    std::cout << "These additional arguments apply when -spectrum (or -S) is used:\n";
    std::cout << l_spec << "\n";
    Result = ON_HELP; }
  
  if (ON_SUCCESS == Result) {
    if (l_vm.count("spectrum")) {
      // Only want peak table output
      use_spectrum (true); }
    // Process the files.
    if (l_vm.count ("file")) {
      Result = process_files (l_vm ["file"].as< std::vector< std::string > >()); }}
  if (ON_HELP == Result) { Result = ON_SUCCESS; }
  return Result;
}

void get_freq::output(const std::string& a_filename) {
  // LOCAL
  boost::filesystem::path l_path (a_filename);
  std::string l_outname (l_path.leaf ());
  // DO
  // Remove any suffix to filename
  const std::string::size_type l_last (l_outname.rfind("."));
  if (std::string::npos != l_last) {
    l_outname.resize (l_last); }
  // Append the new suffix
  if (m_use_spectrum) {
    if (m_spectrum.output_csv ()) { l_outname.append (".csv"); }
    else if (m_spectrum.output_jcampdx ()) { l_outname.append (".dx"); }
    else { l_outname.append (".dat"); }
  }
  else {
    // Plain text data file
    l_outname.append (".dat"); }
  
  // We silently overwrite if file exists.
  std::string l_fullname ((l_path.branch_path() / l_outname).string ());
  
  std::ofstream l_os (l_fullname.c_str());
  if (l_os) {
    serialise (l_os);
    l_os.close(); }
}

void get_freq::serialise(std::ostream & a_os) {
// PRE
// LOCAL
supplier::filter::gaussian_vibration * l_vib_filter = static_cast< supplier::filter::gaussian_vibration * > (m_filter.plugin_object ());
// DO
if (m_use_spectrum) {
  // Use the spectrum
  m_spectrum.clear ();
  m_spectrum.add_peak_table (l_vib_filter->begin (), l_vib_filter->end ());
  m_spectrum.serialise (a_os); }
else {
  // Just output the peak table.
  unsigned int l_flags (chemistry::vibration::PEAK_TABLE);
  if (m_use_raman) {
    l_flags ^= chemistry::vibration::NO_RAMAN; }
  if (m_only_raman) {
    l_flags ^= chemistry::vibration::NO_RAMAN;
    l_flags |= chemistry::vibration::NO_INTENSITY; }
  for (supplier::filter::gaussian_vibration::const_iterator l_i (l_vib_filter->begin ());
      l_i != l_vib_filter->end ();
      ++l_i) {
    l_i->serialise (a_os, ", ", l_flags); }}
}

void get_freq::output_xml(std::ostream & a_os) {
  // We should have now read the entire "gaussian" file. Now we pass the
  // filter object the root data node to add the data to.
  using data_access::namespace purpletree;
  dom::document l_output (dom::implementation::create_document (dom::set_utf8 ("cchem")
        , dom::set_utf8 ("geometry"), dom::set_utf8 ("program.dtd")
        , dom::set_utf8 ("http://cchem.world.org/Version 1.0.0/")));
  
  // Create a rudimentary root node
  dom::element l_geometry (l_output.document_element ());
  l_geometry.set_attribute (dom::set_utf8 ("label"), dom::set_utf8 ("geometry"));
  
  // Add elements for base twiglet
  dom::element l_expt (l_output.create_element (dom::set_utf8 ("experiment")));
  l_geometry.append_child (l_expt);
  l_expt.set_attribute (dom::set_utf8 ("type"), dom::set_utf8 ("calc"));
  l_expt.set_attribute (dom::set_utf8 ("pageref"), dom::set_utf8 ("some-pageref"));
  l_expt.set_attribute (dom::set_utf8 ("worker"), dom::set_utf8 ("some-worker"));
  
  // Get data from filter
  // const int l_bsize (256);
  // char l_errbuf [l_bsize];
  m_filter.output (l_expt.get_node_rep ()/*, l_errbuf, l_bsize)*/);
  if (m_filter.is_error ()) {
    std::cerr << name () << ": " << m_filter.error_message () << "\n"; }
  else {
    // Print
    dom::implementation::serialise (l_output, a_os); }
}

int get_freq::process_files(const std::vector< std::string > & a_fileset) {
  // LOCAL
  const int ON_SUCCESS = 0;
  const int ON_ERROR = 1;
  int Result (ON_SUCCESS);
  // DO
  for (std::vector< std::string >::const_iterator l_i = a_fileset.begin ();
       ON_SUCCESS == Result and l_i != a_fileset.end ();
       ++l_i) {
    std::auto_ptr< supplier::base_filetype > l_file;
    const std::string & l_filename (*l_i);
    CHECK (not l_filename.empty (), "How can you have an empty option?");
    // Test have valid input data
    if (not boost::filesystem::exists (l_filename)) {
      Result = ON_ERROR;
      std::cerr << name () << ": File [" << l_filename << "] does not exist.\n"; }
  
    // open gaussian file.
    if (ON_SUCCESS == Result) {
      l_file.reset (new supplier::filetype::gaussian_file (l_filename));
      if (l_file->bad ()) {
        Result = ON_ERROR;
        std::cerr << name () << ": Unable to open file [" << l_filename << "]\n"; }
      else if (not l_file->is_valid ()){
        Result = ON_ERROR;
        std::cerr << name () << ": File [" << l_filename << "] is not a gaussian log file?\n"; }}
  
    // Process gaussian file
    if (ON_SUCCESS == Result) {
      m_filter.process (*l_file.get ());
      if (not m_filter.is_error ()) {
        output (l_filename); }
      if (m_filter.is_error ()) {
        Result = ON_ERROR;
        std::cerr << name () << " stopped: " << m_filter.error_message () << "\n"; }}}
  return Result;
}


} // namespace cchem::programs

} // namespace cchem

int main (int argc, char ** argv) {
  int Result (0);
  programs::get_freq l_app;
  Result = l_app.main (argc, argv);
  return Result; }
