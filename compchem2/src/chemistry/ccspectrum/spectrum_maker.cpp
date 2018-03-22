// ------------------------------------------------------------
// class: spectrum_maker
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "chemistry/ccspectrum/spectrum_maker.hpp"


int
main (int argc, char **argv)
{
  return spectrum::spectrum_maker < double >::make_spectrum (argc, argv);
}
