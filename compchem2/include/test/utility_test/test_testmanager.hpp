#ifndef COMPCHEM_TEST_TESTMANAGER_HPP
#define COMPCHEM_TEST_TESTMANAGER_HPP

// ------------------------------------------------------------
// class: test_testmanager
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <boost/bind.hpp>

#include <string>

namespace utility {

class test_testmanager : public unit_test {
  private:
    /**
     * Class exemplar
     */
    static test_testmanager exemplar;


  public:
    /**
     * Run tests on test_manager exemplar
     */
    void main_test() const;

    std::string title() const
    {
      return std::string ("Test for unit-test manager class.");
    }
    test_testmanager()
    : unit_test ()
    {}

    virtual ~test_testmanager()
    {}

};

} // namespace utility
#endif
