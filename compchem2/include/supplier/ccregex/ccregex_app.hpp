#ifndef COMPCHEM_CCREGEX_APP_HPP
#define COMPCHEM_CCREGEX_APP_HPP

// ------------------------------------------------------------
// class: ccregex_app
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/basic_application.hpp"
#include <string>

// Manual includes
// -
namespace supplier {

namespace ccregex {

class ccregex_app : public programs::basic_application<ccregex_app>
{
  private:
    /**
     * The name of the file to inspect.
     */
    std::string m_file;

    /**
     * The format string for outputting the regular expression.
     */
    std::string m_format;

    /**
     * The regular expression string
     */
    std::string m_regex;


  public:
    void register_options();

    /**
     * The main method
     */
    int main();

    /**
     * Scan the file.
     * 
     * \pre exists (m_file);
     */
    void scan_file();

    ccregex_app()
    : m_file ()
    , m_format ()
    , m_regex ()
    {}

    ~ccregex_app()
    {}


  private:
    /**
     * no copy
     */
    ccregex_app(const ccregex_app & source);

    /**
     * no assign
     */
    ccregex_app & operator=(const ccregex_app & source);


};

} // namespace supplier::ccregex

} // namespace supplier
#endif
