#ifndef COMPCHEM_PROGRAM_OPTIONS_HPP
#define COMPCHEM_PROGRAM_OPTIONS_HPP

// ------------------------------------------------------------
// class: program_options
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include <boost/program_options/options_description.hpp>

#include <memory>
#include <vector>
#include <boost/program_options/variables_map.hpp>


namespace utility {

/**
 * Manage the program configuration.
 * 
 * The standard operation is:
 * 
 * * .. program initialisation ..
 * ** call helper::get_helper ()... and add options
 * 
 * * .. program main after initialisation ..
 * ** if not helper::empty() # otherwise skip options
 * *** call helper::get_helper().process_options (argc, arv, ??)
 * *** call helper::reset ()
 * * .. rest of program ..
 * 
 */
class program_options
{
  public:
    /**
     * The set of return values from the option parsing methods.
     */
    enum option_result_anonymous
    {
      CCHEM_PARSE_DEFAULT= 0,/**
       * Only pass the command line.
       */

      CCHEM_PARSE_CONFIG_FILE= 1,/**
       * Parse the default configuration file (if not empty). This
       * does not affect configuration files that are set on the 
       * command line as these will always be processed.
       */

      CCHEM_PARSE_ENVIRONMENT= 2,/**
       * Parse the environment for options.
       */

      CCHEM_PARSE_ALL= 3,/**
       * Parse the command line, config file and environment.
       */

      CCHEM_PARSE_SUCCESS= 0,/**
       * Indicate that option processing has completed successfully.
       */

      CCHEM_PARSE_HELP= 1,/**
       * Indicate that the user request the help description. This
       * indicates the program should terminate but without an
       * error indication.
       */

      CCHEM_PARSE_ERROR= 2/**
       * Indicate there was an error processing the options. This
       * normally also prints the help message but indicates the
       * program should terminate with an error condition.
       */


    };


  private:
    /**
     * The main configuration file name.
     */
    std::string m_conf_filename;

    /**
     * The environment variable prefix
     */
    std::string m_env_prefix;

    /**
     * The "hidden" options.
     */
    boost::program_options::options_description m_hidden;

    /**
     * The option name for the positional arguments.
     */
    std::string m_position;

    /**
     * The public options
     */
    boost::program_options::options_description m_public;

    /**
     * The current worker.
     */
    std::string m_worker;

    /**
     * The Singleton helpers.
     */
    static std::auto_ptr<program_options> s_helper;


  public:
    /**
     * Get the name of the configuration file to process.
     */
    const std::string & config_filename() const throw ()
    {
      return m_conf_filename;
    }

    /**
     * Set the name of the configuration file to process.
     */
    void config_filename(std::string a_str)
    {
      m_conf_filename.swap (a_str);
    }

    /**
     * The Id of the current program user.  Generally this is set in the 
     * configuration file or is identical to the current users login name.
     */
    const std::string& current_worker() throw ()
    {
      return m_worker;
    }

    /**
     * Are there no options set? This will be true after calling reset.
     */
    static bool empty() throw ()
    {
      return (NULL == s_helper.get ());
    }
    

    /**
     * Get the environment variable prefix.
     */
    const std::string & environment_prefix() const throw ()
    {
      return m_env_prefix;
    }

    /**
     * Set the environment variable prefix.
     */
    void environment_prefix(std::string a_str)
    {
      m_env_prefix.swap (a_str);
    }

    /**
     * Do we have a positional option set?
     */
    bool have_positional_option() const
    {
      return not m_position.empty ();
    }

    /**
     * Print the help/usage message.  If a_help is true then
     * print the more complete help message, otherwise the
     * shorter usage message
     */
    void help_message(bool a_help, std::ostream & a_log);

    /**
     * Return the helper Singleton. Will create the Singleton if it is not created 
     * already.
     * 
     * You should test whether it is empty() before calling this method when 
     * processing the options.  Once all options are processed you can call
     * reset.
     */
    static program_options & helper();

    /**
     * The set of hidden option descriptions. Hidden options are
     * shown in the help but not the usage message and can not be set
     * via the command-line.
     */
    boost::program_options::options_description & hidden_options() throw()
    {
      return m_hidden;
    }


  private:
    /**
     * no copy
     */
    program_options operator=(const program_options & source);


  public:
    /**
     * Set a_opt_name to be used for all 'positional' arguments
     * (arguments without preceding options).  It can only be set
     * once and must be an existing public option.
     * 
     * Note this system is limited to the common case of one 
     * positional argument that may be used an unlimited number 
     * of times.
     * 
     * \pre not have_positional_option
     * \pre not a_opt_name.empty 
     * \pre public_options ().has (a_opt_name)
     * \post have_positional_option
     */
    void positional_option(const std::string &a_opt_name)
    {
      COMPCHEM_REQUIRE(not have_positional_option (), "You can only set one positional argument");
      COMPCHEM_REQUIRE(not a_opt_name.empty (), "The positional argument must not be an empty string");
      COMPCHEM_REQUIRE(NULL != public_options ().find_nothrow (a_opt_name, true), "Positional option must already be a public option.")
      m_position = a_opt_name;
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
    int process_options(int a_argc, char** a_argv, std::ostream & a_log, unsigned int a_flags = 0);

    /**
     * All-in-one processing. Will parse the command line, configuration file and 
     * environment in that order depending on a_flags, default is command line only.
     * 
     * \param a_argv A vector of command line arguments.
     * \param a_log  The stream to write help/error messages to.
     * \param a_flags Set of flags saying which parsers to use. 
     * \return 0 = success, 1 = help requested, 2 = error.
     */
    int process_options(std::vector< std::string > &a_argv, std::ostream & a_log, unsigned int a_flags = 0);


  private:
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
    int process_options_priv(boost::program_options::variables_map & a_map, std::ostream & a_log, unsigned int a_flags);

    /**
     * Ctor only for Singleton
     */
    program_options();

  public:
    virtual ~program_options();


  private:
    /**
     * no copy
     */
    program_options(const program_options & source);


  public:
    /**
     * The set of public option descriptions.
     */
    boost::program_options::options_description & public_options()
    {
      return m_public;
    }

    /**
     * Once configuration processing is complete, this can be used to 
     * release the allocated memory.
     * 
     * \post empty = true
     */
    static void reset();


};

} // namespace utility
#endif
