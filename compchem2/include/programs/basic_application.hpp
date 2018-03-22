#ifndef COMPCHEM_BASIC_APPLICATION_HPP
#define COMPCHEM_BASIC_APPLICATION_HPP

// ------------------------------------------------------------
// class: basic_application
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/program_options.hpp"

namespace programs {

/**
 * Template class that provides the boilerplate code required for 
 * application initialisation.  It uses the curiously recurring template
 * idiom to allow polymorphism without virtual functions. Derived
 * class need to implement the following functions.
 * 
 * register_options () => Any initialisation required before option
 * processing.
 * int main () => The main part of the application to perform operations
 * after option processing.
 */
template<class t_app>
class basic_application
{
  public:
    virtual ~basic_application() {}

    /**
     * Process the input file.
     */
    int main(int argc, char ** argv);


};
/**
 * Process the input file.
 */
template<class t_app>
int basic_application<t_app>::main(int argc, char ** argv) 
{
  int Result (0);
  try
  {
    ///////////////////////////
    // Process the configuration
  
    // Add program specific options
    static_cast<t_app *>(this)->register_options ();
  
    // Process configuration
    Result = utility::program_options::helper ().process_options (argc, argv, std::cerr, utility::program_options::CCHEM_PARSE_ALL);
  
    // Continue if configuration successful.
    if (utility::program_options::CCHEM_PARSE_SUCCESS == Result)
    {
      Result = static_cast<t_app *>(this)->main ();
    }
  }
  catch (const std::exception & a_err)
  {
    std::cerr << "Caught standard exception.\n" << a_err.what () << "\n";
    Result = 1;
  }
  
  catch (...)
  {
    std::cerr << "Caught unknown non-standard exception\n";
    Result = 1;
  }
  
  return Result;

}


} // namespace programs
#endif
