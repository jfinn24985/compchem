#include <boost/regex.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/scoped_array.hpp>
#include <boost/regex/pattern_except.hpp>
#include <fstream>
#include <iostream>

int main (int argc, char ** argv)
{
  int Result (0);
  if (argc < 3)
  {
    std::cout << argc << " is too few arguments\nUsage is: " << argv[0] << " filename \"re string (boost/extended)\"\n";
    Result = 1;
  }
  else
  {
    try
    {
      std::cout << "Regular expression is : \"" << argv[2] << "\" dummy\n";
      std::cout << "  Supply the third dummy argument if R.E. is 'normal' rather than extended\n";
      // Compile regular expression early, in case of failure.
      boost::regex l_re (argv[2], (argc > 3 ? boost::regex::normal : boost::regex::extended));
      // Read all of file.
      std::ifstream l_infile (argv[1]);
      const unsigned int l_sz (boost::filesystem::file_size (argv[1]));
      boost::scoped_array< char > l_buffer (new char[l_sz]);
      l_infile.read (l_buffer.get (), l_sz);
      if (l_infile.gcount () == static_cast<const int>(l_sz))
      {
        for (boost::regex_iterator< char * > l_M (l_buffer.get (), l_buffer.get () + l_sz, l_re); 
             l_M != boost::regex_iterator< char * > (); ++l_M)
        {
          std::cout << "Match found\n";
          int l_count (0);
          for (boost::match_results< char * >::const_iterator l_i (l_M->begin ()); l_i != l_M->end (); ++l_i)
          {
            std::cout << "Match["<< l_count << "] = " << *l_i << "\n";
            ++l_count;
          }
        }
      }
      else
      {
         std::cout << "File size was " << l_sz << " but read only " << l_infile.gcount () << "\n";
         Result = 1;
      }
    }
    catch (const boost::regex_error &a_err)
    {
      std::cout << "Error compiling regular expression:\n" << a_err.what () << "\n\n";
      Result = 1;
    }
    catch (const std::exception &a_err)
    {
      std::cout << "Caught exception:\n" << a_err.what () << "\n\n";
      Result = 1;
    }
  }
  return Result;
}

