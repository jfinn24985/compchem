#ifndef COMPCHEM_SPECTRUM_MAKER_HPP
#define COMPCHEM_SPECTRUM_MAKER_HPP

// ------------------------------------------------------------
// class: spectrum_maker
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "report/spectrum.hpp"

// Manual includes
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
#include <deque>
// --

//namespace std
//{
//template<typename charT, typename traits, typename T1, typename T2>
//inline std::basic_istream<charT, traits>& operator>>(std::basic_istream<charT, traits> &in, std::pair<T1, T2> &p)
//{
//  return in >> p.first >> p.second;
//}
//}
namespace spectrum {

template<class t_value>
class spectrum_maker
{
  public:
    typedef t_value value_type;

    /**
     * The spectrum generator manager class.
     */
    typedef report::spectrum< value_type > option_type;

    /**
     * Generate the spectrum
     */
    static int make_spectrum(int argc, char ** argv);


};
/**
 * Generate the spectrum
 */
template<class t_value>
int spectrum_maker<t_value>::make_spectrum(int argc, char ** argv)

{
  int Result (0);
  report::spectrum < value_type > l_result;
  std::string l_input_filename;
  std::string l_output_filename;
  std::vector < std::string > l_filenames;
  bool l_verbose (false);
  unsigned int
  l_filenames_top (0);
  // Set up the program option processing.
  boost::program_options::options_description l_desc ("Allowed options");	// Public options
  boost::program_options::options_description l_all ("All options");	// Public and hidden options
  boost::program_options::positional_options_description l_bare_arg;
  boost::program_options::variables_map l_vm;
  l_desc.add_options ()("help,h", "produce help");
  l_desc.add_options ()("usage,?", "also produce help");
  l_desc.add_options ()("input,I", boost::program_options::value < std::string > (&l_input_filename), "input filename (if not specified then standard input is used or the first non-option argument)");
  l_desc.add_options ()("output,o", boost::program_options::value < std::string > (&l_output_filename), "output filename (if not specified then a non-option argument is used or standard output)");
  l_desc.add_options ()("verbose,v", boost::program_options::bool_switch (&l_verbose), "Write information messages to standard error");
  l_result.register_options (l_desc);
  l_all.add_options ()("@files", boost::program_options::value < std::vector < std::string > >(&l_filenames), "hidden filename option");
  // Combine arguments
  l_all.add (l_desc);
  l_bare_arg.add ("@files", 2);
  // Attempt to process options.
  try
  {
    boost::program_options::store (boost::program_options::command_line_parser (argc, argv).options (l_all).positional (l_bare_arg).run (), l_vm);
  }
  catch (const std::exception & a_err)
  {
    std::cout << "Error: " << a_err.what () << "\n";
    std::cout << l_desc << "\n";
    Result = 1;
  }
  //////////////////
  // HELP
  if (0 == Result)
  {
    // Check is help is wanted.
    if (l_vm.count ("help") or l_vm.count ("usage"))
    {
      std::cout << l_desc << "\n";
      Result = 1;
    }
  }
  //////////////////
  // INPUT
  if (0 == Result)
  {
    // All options accepted and we don't want any help -> Process any option notifiers.
    boost::program_options::notify (l_vm);
    // Where is the data source?
    if (not l_input_filename.empty ())
    {
      // If we have a filename open it for reading.
      std::ifstream l_is (l_input_filename.c_str ());
      l_result.read_peak_table (l_is);
    }
    else
    {
      // Check the positional filename, assume we read it if a file exists
      if (l_filenames_top < l_filenames.size () and boost::filesystem::exists (l_filenames.at (l_filenames_top)))
      {
        if (l_verbose)
        {
          std::cerr << "Attempting to use file " << l_filenames.at (l_filenames_top) << " for peak table.\n";
        }
        // If we have a filename open it for reading.
        std::ifstream l_is (l_filenames.at (l_filenames_top).c_str ());
        l_result.read_peak_table (l_is);
        ++l_filenames_top;
      }
      else
      {
        // If there is anything on standard input?
        if (l_verbose)
        {
          std::cerr << "Attempting to use standard input for peak table.\n";
        }
        l_result.read_peak_table (std::cin);
      }
    }
  }
  /////////////////////
  // OUTPUT
  if (0 == Result)
  {
  
    // Where do we write?
    if (not l_output_filename.empty ())
    {
      // If we have a filename open it for reading.
      std::ofstream l_os (l_output_filename.c_str ());
      l_result.serialise (l_os);
    }
    else
    {
      // Use the positional filename
      if (l_filenames_top < l_filenames.size ())
      {
        if (l_verbose)
        {
          std::cerr << "Attempting to use file " << l_filenames.at (l_filenames_top) << " for output.\n";
        }
        // If we have a filename open it for reading.
        std::ofstream l_os (l_filenames.at (l_filenames_top).c_str ());
        l_result.serialise (l_os);
        ++l_filenames_top;
      }
      else
      {
        if (l_verbose)
        {
          std::cerr << "Attempting to use standard output for peak table.\n";
        }
        // Use standard output as last resort
        l_result.serialise (std::cout);
      }
    }
  }
  return Result;

}


} // namespace spectrum
#endif
