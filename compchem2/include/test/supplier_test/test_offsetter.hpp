#ifndef COMPCHEM_TEST_OFFSETTER_HPP
#define COMPCHEM_TEST_OFFSETTER_HPP

// ------------------------------------------------------------
// class: test_offsetter
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/unit_test.hpp"
#include <string>

namespace supplier {

class test_offsetter : public utility::unit_test {
  private:
    /**
     * The class exemplar.
     */
    static test_offsetter s_exmplr;


  public:
    /**
     * Override in derived classes to perform unit test.
     */
    void main_test() const;

    test_offsetter() throw()
    : unit_test ()
    {}

    ~test_offsetter() throw()
    {}

    std::string title() const
    {
      return "offsetter test";
    }

};

} // namespace supplier
#endif
