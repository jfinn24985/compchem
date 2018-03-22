// ------------------------------------------------------------
// class: program_options
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "utility/program_options.hpp"

// Manual includes
#include <boost/filesystem/operations.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#ifdef HAVE_UNISTD_H
#include "utility/filesystem_properties.hpp"
#endif
// -
namespace utility {

/**
 * The Singleton helpers.
 */
std::auto_ptr<program_options> program_options::s_helper;

/**
 * Print the help/usage message.  If a_help is true then
 * print the more complete help message, otherwise the
 * shorter usage message
 */
void program_options::help_message(bool a_help, std::ostream & a_log) 
{
  if (a_help)
  {
    a_log << "Help:\n";
  }
  else
  {
    a_log << "usage:\n";
  }
  a_log << m_public << "\n";
  if (a_help)
  {
    a_log << "The following options can only be set in the configuration file or environment:\n";
    a_log << m_hidden << "\n";
  }

}

/**
 * Return the helper Singleton. Will create the Singleton if it is not created 
 * already.
 * 
 * You should test whether it is empty() before calling this method when 
 * processing the options.  Once all options are processed you can call
 * reset.
 */
program_options & program_options::helper()

{
  if (NULL == s_helper.get ()) { s_helper.reset (new program_options); }
  return *(s_helper.get ());
}

/**
 * All-in-one processing. Will parse the command line, configuration file and 
 * environment in that order depending on a_flags, default is command line only.
 * 
 * \param a_argc The length of the C zstring array argv.
 * \param a_argv The C zstring array of command line arguments.
 * \param a_log  The stream to write help/error messages to.
 * \param a_flags Set of flags saying which parsers to use. 
 * \return 0 = success, 1 = help requested, 2 = error.
 */
int program_options::process_options(int a_argc, char** a_argv, std::ostream & a_log, unsigned int a_flags) 
{
  // local
  int Result (CCHEM_PARSE_SUCCESS);
  boost::program_options::variables_map l_map;
  ///////////////////////////
  // Add program specific options
  m_public.add_options ()("help,h", "print long help")("usage,?", "print quick help")("config,C", "local configuration file");
  
  try
  {
    ///////////////////////////
    // Process the command line
    boost::program_options::positional_options_description l_pos_args;
    l_pos_args.add(m_position.c_str (), -1);
    boost::program_options::store (boost::program_options::command_line_parser (a_argc, a_argv).options (m_public).positional (l_pos_args).run (), l_map);
    ///////////////////////////
    // Do the rest of the configuration
    Result = process_options_priv (l_map, a_log, a_flags);
  }
  catch (const std::exception & a_err)
  {
    // Assume option parsing error so print help.
    Result = CCHEM_PARSE_ERROR;
    a_log << "Error: " << a_err.what () << "\n";
  }
  
  ///////////////////////////
  // Print help if required
  if (0 != l_map.count ("help") or 0 != l_map.count ("usage"))
  {
    Result = CCHEM_PARSE_HELP;
    help_message (0 != l_map.count ("help"), a_log);
  }
  
  return Result;

}

/**
 * All-in-one processing. Will parse the command line, configuration file and 
 * environment in that order depending on a_flags, default is command line only.
 * 
 * \param a_argv A vector of command line arguments.
 * \param a_log  The stream to write help/error messages to.
 * \param a_flags Set of flags saying which parsers to use. 
 * \return 0 = success, 1 = help requested, 2 = error.
 */
int program_options::process_options(std::vector< std::string > &a_argv, std::ostream & a_log, unsigned int a_flags) 
{
  // local
  int Result (CCHEM_PARSE_SUCCESS);
  boost::program_options::variables_map l_map;
  ///////////////////////////
  // Add program specific options
  m_public.add_options ()("help,h", "print long help")("usage,?", "print quick help")("config,C", "local configuration file");
  
  try
  {
    ///////////////////////////
    // Process the command line
    boost::program_options::positional_options_description l_pos_args;
    l_pos_args.add(m_position.c_str (), -1);
    boost::program_options::store (boost::program_options::command_line_parser (a_argv).options (m_public).positional (l_pos_args).run (), l_map);
    ///////////////////////////
    // Do the rest of the configuration
    Result = process_options_priv (l_map, a_log, a_flags);
  }
  catch (const std::exception & a_err)
  {
    // Assume option parsing error so print help.
    Result = CCHEM_PARSE_ERROR;
    a_log << "Error: " << a_err.what () << "\n";
  }
  
  ///////////////////////////
  // Print help if required
  if (0 != l_map.count ("help") or 0 != l_map.count ("usage"))
  {
    Result = CCHEM_PARSE_HELP;
    help_message (0 != l_map.count ("help"), a_log);
  }
  
  return Result;

}

/**
 * All-in-one processing implemenation. This part of the process_options 
 * functions parses the config file and the environment
 * 
 * \param a_argc The length of the C zstring array argv.
 * \param a_argv The C zstring array of command line arguments.
 * \param a_log  The stream to write help/error messages to.
 * \param a_flags Set of flags saying which parsers to use. 
 * \return 0 = success, 1 = help requested, 2 = error.
 */
int program_options::process_options_priv(boost::program_options::variables_map & a_map, std::ostream & a_log, unsigned int a_flags) 
{
  // local
  int Result (CCHEM_PARSE_SUCCESS);
  ///////////////////////////
  // Process the configuration files
  if (0 != a_map.count ("config"))
  {
    const std::string l_filename (a_map["config"].as < std::string > ().c_str ());
    if (boost::filesystem::exists (l_filename))
    {
      // Assemble the program options
      boost::program_options::options_description l_all_opt ("All options");
      l_all_opt.add (m_public);
      l_all_opt.add (m_hidden);
      std::ifstream l_conf (l_filename.c_str ());
      boost::program_options::store (boost::program_options::parse_config_file (l_conf, l_all_opt), a_map);
    }
    else
    {
      Result = CCHEM_PARSE_ERROR;
      a_log << "Error: Configuration file [" << l_filename << "] on command line does not exist.\n";
    }
  }
  if (Result == CCHEM_PARSE_SUCCESS and 0 != (CCHEM_PARSE_CONFIG_FILE & a_flags) and not m_conf_filename.empty ())
  {
    if (boost::filesystem::exists (m_conf_filename.c_str ()))
    {
      // Assemble the program options
      boost::program_options::options_description l_all_opt ("All options");
      l_all_opt.add (m_public);
      l_all_opt.add (m_hidden);
      std::ifstream l_conf (m_conf_filename.c_str ());
      boost::program_options::store (boost::program_options::parse_config_file (l_conf, l_all_opt), a_map);
    }
    else
    {
      Result = CCHEM_PARSE_ERROR;
      a_log << "Error: Default configuration file [" << m_conf_filename << "] does not exist.\n";
    }
  }
    ///////////////////////////
    // Process the environment
  if (Result == CCHEM_PARSE_SUCCESS and 0 != (CCHEM_PARSE_ENVIRONMENT & a_flags) and not m_env_prefix.empty ())
  {
    // Assemble the program options
    boost::program_options::options_description l_all_opt ("All options");
    l_all_opt.add (m_public);
    l_all_opt.add (m_hidden);
    boost::program_options::store (boost::program_options::parse_environment (l_all_opt, m_env_prefix), a_map);
  }
  if (Result == CCHEM_PARSE_SUCCESS)
  {
    boost::program_options::notify (a_map);
  }
  return Result;

}

/**
 * Ctor only for Singleton
 */
program_options::program_options() 
: m_conf_filename ()
, m_env_prefix ()
, m_hidden ()
, m_position ()
, m_public ()
, m_worker ()
{
#ifdef HAVE_UNISTD_H
  m_worker = utility::filesystem_properties::current_user ();
#endif
  m_public.add_options ()("worker", boost::program_options::value < std::string >(&m_worker), "The Id of the current worker.");
}



program_options::~program_options() 
{}

/**
 * Once configuration processing is complete, this can be used to 
 * release the allocated memory.
 * 
 * \post empty = true
 */
void program_options::reset()

{
  s_helper.reset ();
  COMPCHEM_ENSURE(program_options::empty (), "Program options not empty after a reset.");
}


} // namespace utility
