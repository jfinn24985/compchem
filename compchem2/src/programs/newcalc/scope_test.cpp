// ------------------------------------------------------------
// class: scope_test
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "programs/newcalc/scope_test.hpp"
#include "programs/newcalc/scope.hpp"
#include "programs/newcalc/newcalc.hpp"

namespace cchem {

namespace newcalc {

scope_test::scope_test() {
}

scope_test::~scope_test() {
}

/**
 * Override in derived classes to perform unit test.
 */
int scope_test::main_test(int argc, char ** argv) const {
  // LOCAL
  int Result (0);
  // DO
  cchem::newcalc::newcalc::guistate_type l_app (argc, argv);
  scope_working l_top (l_app);
  scope_local l_loc (l_top);
  
  std::cout << l_loc.value ("*.configuration.algorithm.options");
  // END
  return Result;
}


} // namespace cchem::newcalc

} // namespace cchem
