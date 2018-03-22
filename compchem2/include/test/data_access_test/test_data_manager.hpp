#ifndef COMPCHEM_TEST_DATA_MANAGER_HPP
#define COMPCHEM_TEST_DATA_MANAGER_HPP

// ------------------------------------------------------------
// class: test_data_manager
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace data_access { class manager; } 

namespace data_access {

/**
 * Test the data_access::manager class
 */
class test_data_manager : public utility::unit_test
{
  private:
    static test_data_manager s_exemplar;


  public:
    /**
     * Override in derived classes to perform unit test.
     * 
     * Failed tests are indicated by an uncaught exception.
     */
    void main_test() const;

    std::string title() const
    {
      return std::string ("Test of data_access::manager class.");
    }


};

} // namespace data_access
#endif
