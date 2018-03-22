// ------------------------------------------------------------
// class: application_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/application_py.hpp"
#include "utility/program_options.hpp"

// Manual source includes
#include <boost/python.hpp>
#include <boost/python/extract.hpp>
// -
namespace programs {

/**
 * Process the command line/config file etc.
 */
int application_py::initialise(const boost::python::list & a_argv) 
{
  ///////////////////////////
  // Process the configuration
  std::vector< std::string > l_argv;
  const int l_len = boost::python::extract< int >(a_argv.attr ("__len__")());
  for (int l_i (0); l_i < l_len; ++l_i)
  {
    const char* l_tmp = boost::python::extract< const char* >(a_argv [l_i]);
    l_argv.push_back (std::string (l_tmp));
  }
  
  return utility::program_options::helper ().process_options (l_argv, std::cerr, utility::program_options::CCHEM_PARSE_ALL);
  

}

/**
 * Add an option defined by a_opt to the application. Include a_help message
 * in the usage message.
 */
void application_py::register_option(const std::string& a_opt, char a_letter, const std::string& a_help) 
{
  std::string l_opts (a_opt);
  l_opts.reserve (l_opts.size () + 2);
  l_opts.push_back (',');
  l_opts.push_back (a_letter);
  utility::program_options::helper ().public_options ().add_options ()(l_opts.c_str (), boost::program_options::value < std::string > ()->notifier (boost::bind (&application_py::set_option, this, a_opt, _1)), a_help.c_str ());
  

}

// This file defines the python wrappers for the program namespace
// into the program python module.

BOOST_PYTHON_MODULE(program)
{
 boost::python::class_< programs::application_py, boost::noncopyable >("application",
        "Initialisation manager class for the compchem system.",
        boost::python::init<>()
        )
 .def("initialise", &programs::application_py::initialise, 
         "Perform application initialisation. This parses the command line, the \n"
         "configuration file and the environment.")
 .def("register_option", &programs::application_py::register_option,
         "Add an initialisation option. This only makes sense before calling initialise.")
 .def("option_value", &programs::application_py::option_value,
         "Get the value of an option. This only makes sense after calling initialise.")
 ; 
}


} // namespace programs
