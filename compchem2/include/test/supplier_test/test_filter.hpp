#ifndef COMPCHEM_TEST_FILTER_HPP
#define COMPCHEM_TEST_FILTER_HPP

// ------------------------------------------------------------
// class: test_filter
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include "supplier/basic_filter.hpp"
#include <string>

namespace supplier {

/**
 * UNUSED
 */
class test_filter : public utility::unit_test {
  private:
    /**
     * The exemplar
     */
    static test_filter s_exmplr;


  public:
    /**
     * Override in derived classes to perform unit test.
     */
    void main_test() const;

    /**
     * Override in derived classes to perform unit test.
     */
    void main_test2() const;

    std::string title() const {
      return "Test filter objects"; }

};

} // namespace supplier
#endif
