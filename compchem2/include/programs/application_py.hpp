#ifndef COMPCHEM_APPLICATION_PY_HPP
#define COMPCHEM_APPLICATION_PY_HPP

// ------------------------------------------------------------
// class: application_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/bind.hpp>

#include <map>
#include <string>

// Manual header includes
#include <boost/python/list.hpp>
// -
namespace utility { class program_options; } 

namespace programs {

class application_py
{
  private:
    /**
     * The map of initialisation results..
     */
    std::map<std::string, std::string> m_map;


  public:
    application_py()
    : m_map ()
    {}


  private:
    /**
     * no copy
     */
    application_py(const application_py &);


  public:
    ~application_py()
    {}

    /**
     * Process the command line/config file etc.
     */
    int initialise(const boost::python::list & a_argv);


  private:
    /**
     * no assign
     */
    application_py & operator=(const application_py &);


  public:
    /**
     * Get a value set for a_opt (if any).
     */
    std::string option_value(const std::string& a_opt)
    {
      return m_map [a_opt];
    }
    /**
     * Add an option defined by a_opt to the application. Include a_help message
     * in the usage message.
     */
    void register_option(const std::string& a_opt, char a_letter, const std::string& a_help);

    /**
     * Set a configuration option.
     */
    void set_option(const std::string& a_opt, const std::string& a_value)
    {
      m_map [a_opt] = a_value;
    }


};

} // namespace programs
#endif
