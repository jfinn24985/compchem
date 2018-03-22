#ifndef COMPCHEM_TEST_SUPPLIER_HELPER_HPP
#define COMPCHEM_TEST_SUPPLIER_HELPER_HPP

// ------------------------------------------------------------
// class: test_supplier_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace utility { class scoped_call; } 
namespace supplier { class supplier_helper; } 

namespace supplier {

/**
 * Test case to try out the supplier_helper.
 */
class test_supplier_helper : public utility::unit_test {
  private:
    /**
     * Class exemplar
     */
    static test_supplier_helper s_exmplr;


  public:
    /**
     * Override in derived classes to perform unit test.
     */
    void main_test() const;

    std::string title() const
    {
      return "Test /compchem/supplier//supplier_helper Singleton class";
    }

    test_supplier_helper()
    : unit_test ()
    {}

    ~test_supplier_helper() throw()
    {}

};

} // namespace supplier
#endif
