// ------------------------------------------------------------
// class: plugin_filter_direct
// Orig author: Justin Finnerty
// $author:$
// Orig date:
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/plugin_filter_direct.hpp"

namespace supplier {

/**
 * Create a filter to operate on the given file.
 * 
 * \post error = true if an error occurred
 */
plugin_filter_direct::plugin_filter_direct()
  : m_error (0)
  , m_error_msg () {
  if (&::plugin_object == NULL) {
    m_error = 2;
    m_error_msg << "Could not find valid filter object.\n"; }
  else {
    plugin (&::plugin_object);
    // Object created was invalid!
    if (not is_valid ()) {
      m_error = 2;
      m_error_msg << "Found plugin object is not valid!"; }}}



} // namespace supplier

// int main (int argc, char ** argv) {
//   int Result (0);
//   if (argc > 1) {
//     for (int i (0); i < argc; ++i)
//       std::cout << "argv[" << i << "] = [" << argv[i] << "]\n";
//     supplier::plugin_filter_direct l_filter;
//     supplier::filetype::gaussian_file l_file (argv[1]);
//     if (l_file.is_valid ()) {
//       std::cout << "FILETYPE: " << l_file.filetype () << "\n";
//       std::cout << "SUBTYPE:  " << l_file.subtype_info () << "\n";
//       l_filter.process (l_file);
//       if (not l_filter.is_error ()) {
// 	supplier::filter::gaussian_vibration* l_vib_filter = static_cast<supplier::filter::gaussian_vibration*>(l_filter.plugin_object ());
// 	std::cout << "SIZE: " << l_vib_filter->size () << "\n";
//         for (supplier::filter::gaussian_vibration::const_iterator l_i (l_vib_filter->begin ());
//             l_i != l_vib_filter->end ();
//             ++l_i) {
// 	    l_i->serialise (std::cout, ",", chemistry::vibration::PEAK_TABLE); }}
//       else {
//         std::cout << "ERROR: " << l_filter.error_message () << "\n"; }}
//     else {
//       std::cout << "File " << argv[1] << " is not a valid gaussian file\n"; }}
//   else {
//     std::cout << "Usage: get_freq filtename\n"; }
//   return Result; }
