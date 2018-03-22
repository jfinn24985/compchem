#ifndef COMPCHEM_TEST_CONTRACT_ERROR_HPP
#define COMPCHEM_TEST_CONTRACT_ERROR_HPP

// ------------------------------------------------------------
// class: test_contract_error
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include "utility/contract_error.hpp"
#include <string>

namespace utility {

/**
 * Unit test for the contract error class and the design-by-contract system 
 * macros.
 */
class test_contract_error : public unit_test {
  public:
    virtual std::string title() const {
      return "Test contract_error class."; }

    /**
     * Override in derived classes to perform unit test.
     */
    void main_test() const;

    test_contract_error()
    : unit_test ()
    {}

    virtual ~test_contract_error()
    {}


  private:
    /**
     * The class exemplar
     */
    static test_contract_error exemplar;

};

} // namespace utility
#endif
