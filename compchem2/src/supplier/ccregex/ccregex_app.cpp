// ------------------------------------------------------------
// class: ccregex_app
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/ccregex/ccregex_app.hpp"
#include <boost/regex.hpp>


// Manual includes
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
// -
int main (int argc, char ** argv)
{
  supplier::ccregex::ccregex_app l_app;
  return l_app.programs::basic_application< supplier::ccregex::ccregex_app >::main (argc, argv);
}
namespace supplier {

namespace ccregex {

void ccregex_app::register_options() 
{
  // Add option
  utility::program_options::helper ().public_options ().add_options ()
   ("file", boost::program_options::value < std::string > (&m_file), "Text file to compare regex against")
   ("regex", boost::program_options::value < std::string > (&m_regex), "The regular expression to test (non-interactive mode)");

}

/**
 * The main method
 */
int ccregex_app::main() 
{
  int Result (0);
  if (not boost::filesystem::exists (m_file))
  {
    std::cerr << "File \"" << m_file << "\" does not exist.\n";
    Result = 1;
  }
  else
  {
    if (not m_regex.empty ())
    {
      scan_file ();
    }
    else
    {
      while (true)
      {
        std::cout << "Enter regular expression (empty line exits)\n";
        std::getline (std::cin, m_regex);
        if (m_regex.empty ())
        {
          break;
        }
        std::cout << "Enter format (optional)\n";
        std::getline (std::cin, m_format);
        std::cout << "Got regex [" << m_regex << "],\n   format [" << m_format << "]\n";
        try
        {
          scan_file ();
        }
        catch (const std::exception& a_err)
        {
          std::cerr << "Caught exception: " << a_err.what ();
        }
      }
    }
  }
  return Result;

}

/**
 * Scan the file.
 * 
 * \pre exists (m_file);
 */
void ccregex_app::scan_file() 
{
  // scan once and exit.
  static boost::iostreams::mapped_file_source l_source (m_file);
  COMPCHEM_ALWAYS (l_source.is_open (), "Failed to open file " + m_file);
  boost::regex l_re (m_regex);
  boost::cmatch l_result;
  if (boost::regex_search (l_source.data ()
                           , l_source.data () + l_source.size ()
                           , l_result
                           , l_re))
  {
    for (unsigned int l_ix = 0; l_ix < l_result.size (); ++l_ix)
    {
      std::cout << "match [" << l_ix << "] [" << l_result[l_ix] << "]\n";
    }
    if (not m_format.empty ())
    {
      std::cout << "result [" << l_result.format (m_format) << "]\n";
    }
  }
  else
  {
    std::cout << "no match\n";
  }

}


} // namespace supplier::ccregex

} // namespace supplier
