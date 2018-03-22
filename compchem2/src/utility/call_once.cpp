// ------------------------------------------------------------
// class: call_once
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "utility/call_once.hpp"

namespace utility {

/**
 * This uses the empty ctor once to create the T_once Singleton.
 */
call_once::call_once(boost::function< void (void) > a_functor) 
{
  if (0 == count++)
  {
    a_functor ();
  }
}

call_once::~call_once() 
{
  --count;
}

int call_once::count(0);


} // namespace utility
