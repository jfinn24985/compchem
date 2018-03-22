#ifndef COMPCHEM_TEST_NAMES_HPP
#define COMPCHEM_TEST_NAMES_HPP

// ------------------------------------------------------------
// class: test_names
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace data_access { template<class T_xml_string> class basic_names; } 

namespace data_access {

/**
 * Unit test for data_access::names.
 */
class test_names : public utility::unit_test {
  private:
    /**
     * Class exemplar
     */
    static test_names exemplar;


  public:
    /**
     * (1) Test that the data_access::names have the expected values and 
     * that they are categorised correctly.
     * (2) Test the list methods (which internally use list_add)
     */
    void main_test() const;

    test_names()
    : utility::unit_test ()
    {}

    ~test_names()
    {}

    std::string title() const
    {
      return std::string ("data_access::names class test");
    }

};

} // namespace data_access
#endif
