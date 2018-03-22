// ------------------------------------------------------------
// class: newcalc_main
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------

#include "programs/newcalc/newcalc.hpp"

int main (int argc, char** argv) {
  cchem::newcalc::newcalc::guistate_type l_appl (argc, argv);
  l_appl.display (); }
