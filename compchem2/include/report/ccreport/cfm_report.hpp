#ifndef COMPCHEM_CFM_REPORT_HPP
#define COMPCHEM_CFM_REPORT_HPP

// ------------------------------------------------------------
// class: cfm_report
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/basic_application.hpp"
#include <string>

namespace data_scheme { class project; } 
namespace data_scheme { class program; } 
namespace report { class project_cfm_report; } 
namespace data_scheme { template<class t_src_type> class scoped_open; } 

namespace ccreport {

class cfm_report : public programs::basic_application<cfm_report>
{
  private:
    /**
     * The name of the database to query.
     */
    std::string m_database;

    std::string m_output;


  public:
    /**
     * Override in derived classes to perform unit test.
     */
    int main() const;

    /**
     * This method is called by the manager to allow test types to 
     * register program options.  The default implementation does
     * nothing.
     */
    void register_options();

    cfm_report()
    : m_database ()
    , m_output ()
    {}

    ~cfm_report()
    {}


  private:
    /**
     * no copy
     */
    cfm_report(const cfm_report & source);

    /**
     * no assign
     */
    cfm_report & operator=(const cfm_report & source);


};

} // namespace ccreport
#endif
