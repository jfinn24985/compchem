#ifndef COMPCHEM_UNIT_TEST_HPP
#define COMPCHEM_UNIT_TEST_HPP

// ------------------------------------------------------------
// class: unit_test
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/test_manager.hpp"

namespace utility {

/**
 * Base class for unit test classes.
 * 
 * It is designed so that derived classes need only create a static object of 
 * themselves to register with the test-manager class.
 */
class unit_test : public base_test {
  protected:
    unit_test()
    {
      test_manager::get_exemplar ().add_tester (this);
    }


  public:
    virtual ~unit_test()
    {
      test_manager::get_exemplar ().remove_tester (this);
    }


  private:
    /**
     * Not copyable
     */
    unit_test(const unit_test & source);

    /**
     * Not copyable
     */
    unit_test & operator=(const unit_test & source);

};

} // namespace utility
#endif
