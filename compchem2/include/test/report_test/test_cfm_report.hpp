#ifndef COMPCHEM_TEST_CFM_REPORT_HPP
#define COMPCHEM_TEST_CFM_REPORT_HPP

// ------------------------------------------------------------
// class: test_cfm_report
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace data_scheme { class program; } 
namespace report { class project_cfm_report; } 

namespace report {

class test_cfm_report : public utility::unit_test
{
  private:
    /**
     * The name of the database to query.
     */
    std::string m_database;

    /**
     * The name of a file containing the expected output.
     */
    std::string m_test_file;

    /**
     * The Singleton test object.
     */
    static test_cfm_report s_exemplar;


  public:
    std::string title() const
    {
      return std::string ("Test report//project_cfm_report class.");
    }

    /**
     * Override in derived classes to perform unit test.
     */
    void main_test() const;


  private:
    /**
     * This method is called by the manager to allow test types to 
     * register program options.  The default implementation does
     * nothing.
     */
    void register_options();

    test_cfm_report()
    : m_database ()
    , m_test_file ()
    {
      register_options ();
    }


  public:
    ~test_cfm_report()
    {}


  private:
    /**
     * no copy
     */
    test_cfm_report(const test_cfm_report & source);

    /**
     * no assign
     */
    test_cfm_report & operator=(const test_cfm_report & source);


};

} // namespace report
#endif
