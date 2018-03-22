#ifndef COMPCHEM_CONFIG_HPP
#define COMPCHEM_CONFIG_HPP

// ------------------------------------------------------------
// class: config
// $revision:$
// $log:$
// ------------------------------------------------------------

/**
 * This header file contains macro definitions and code for the design by contract system.  Currently this is all inlined code.
 */
#include "requirements.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/shared_ptr.hpp>
#include "utility/contract_error.hpp"

namespace utility {

class implementation {
  public:
    /**
     * Change the file that std::clog points to.
     */
    static void change_log(const std::string & a_filename)
    {
      boost::shared_ptr < std::fstream > l_tmp (new std::fstream (a_filename.c_str (), std::ios::out | std::ios::trunc));
      log_impl () = l_tmp;
    }

    /**
     * Return a reference to the error stream. (Defaults to std::cerr)
     */
    static inline std::ostream& errlog()
    {
      return (log_impl ().get ())? *(log_impl ().get ()) : std::cerr;
    }
    

    /**
     * Return a reference to the log stream. (Defaults to std::clog)
     */
    static inline std::ostream& log()
    {
      return (log_impl ().get ())? *(log_impl ().get ()) : std::cout;
    }
    

    static boost::shared_ptr < std::ostream >& log_impl()
    {
      static boost::shared_ptr < std::ostream > s_log;
      return s_log;
    }

    /**
     * Print a formatted error message.  Primarily designed to operate with LOG macro.
     */
    static inline void log_message(const std::string & a_msg, const char * a_file, int a_line, const char * a_fn_name=NULL)
    {
      log() << "LOG: \"" << a_msg << "\" (" << a_file << ":" << a_line;
      if (NULL != a_fn_name)
      {
        log () << ":" << a_fn_name;
      }
      log() << ")\n";
    }

};

} // namespace utility

#ifndef COMPCHEM_FUNCTION_NAME
// Attempt to provide a meaningful function name on systems that support it
# if defined __cplusplus && __GNUC_PREREQ(2, 6)
#   define COMPCHEM_FUNCTION_NAME	__PRETTY_FUNCTION__
# else
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#   define COMPCHEM_FUNCTION_NAME	__func__
#  else
#   define COMPCHEM_FUNCTION_NAME	((const char *) 0)
#  endif
# endif
#endif
#ifndef NO_LOG
#define LOG(X) { ::utility::implementation::log_message (X, __FILE__, __LINE__, COMPCHEM_FUNCTION_NAME); }
#define DLOG(X) { ::utility::implementation::log () << (X); } 
#define COMPCHEM_LOG(X) { ::utility::implementation::log_message (X, __FILE__, __LINE__, COMPCHEM_FUNCTION_NAME); }
#define COMPCHEM_DLOG(X) { ::utility::implementation::log () << (X); } 
#else
#define LOG(X)
#define DLOG(X)
#define COMPCHEM_LOG(X)
#define COMPCHEM_DLOG(X)
#endif

// Define a macro that does nothing!
#define COMPCHEM_NOOP(X) X

#endif
