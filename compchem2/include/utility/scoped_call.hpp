#ifndef COMPCHEM_SCOPED_CALL_HPP
#define COMPCHEM_SCOPED_CALL_HPP

// ------------------------------------------------------------
// class: scoped_call
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/function.hpp>


// Manual includes
// -
namespace utility {

/**
 * This class ensures that some function is called even in the event
 * of an exception.
 */
class scoped_call
{
  public:
    typedef boost::function0<void> function_type;


  private:
    /**
     * The function to call at scope exit.
     */
    function_type m_fn;


  public:
    /**
     * Save the function for scope exit.
     */
    scoped_call(function_type a_fn)
    : m_fn (a_fn)
    {}

    /**
     * Save the function for scope exit.
     */
    scoped_call(function_type & a_fn)
    : m_fn ()
    {
      m_fn.swap (a_fn);
    }

    /**
     * Call the stored functor
     */
    ~scoped_call()
    {
      if (not m_fn.empty ()) m_fn ();
    }

  private:
    /**
     * no copy
     */
    scoped_call(const scoped_call & source);

    /**
     * no assign
     */
    scoped_call & operator=(const scoped_call & source);


};

} // namespace utility
#endif
