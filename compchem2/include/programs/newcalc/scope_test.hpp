#ifndef COMPCHEM_NEWCALC_SCOPE_TEST_HPP
#define COMPCHEM_NEWCALC_SCOPE_TEST_HPP

// ------------------------------------------------------------
// class: scope_test
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

namespace cchem { namespace newcalc { class scope_working; }  } 
namespace cchem { namespace newcalc { class newcalc; }  } 

namespace cchem {

namespace newcalc {

class scope_test : public utility::unit_test {
  public:
    scope_test();

    virtual ~scope_test();

    std::string title() const {
      return std::string ("Scope Test"); }

    /**
     * Override in derived classes to perform unit test.
     */
    int main_test(int argc, char ** argv) const;

};

} // namespace cchem::newcalc

} // namespace cchem
#endif
