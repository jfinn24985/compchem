#ifndef COMPCHEM_CALL_ONCE_HPP
#define COMPCHEM_CALL_ONCE_HPP

// ------------------------------------------------------------
// class: call_once
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/function.hpp>


namespace utility {

/**
 * Class to ensure that some functor is called exactly once.
 * 
 * Use the following in a header to ensure (*function_addr)()
 * is called only once.
 * static once_init< &function_addr > a_dummy_name__;
 */
class call_once
{
  public:
    /**
     * This uses the empty ctor once to create the T_once Singleton.
     */
    call_once(boost::function< void (void) > a_functor);

    ~call_once();


  private:
    /**
     * no copy
     */
    call_once(const call_once & source);

    static int count;

    /**
     * no assign
     */
    call_once & operator=(const call_once & source);


};

} // namespace utility
#endif
