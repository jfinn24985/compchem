#ifndef COMPCHEM_ONCE_INIT_HPP
#define COMPCHEM_ONCE_INIT_HPP

// ------------------------------------------------------------
// class: once_init
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "utility/friend_maker.hpp"

namespace utility {

/**
 * Class to ensure that some exemplar is constructed
 * before it is first accessed AND that there is only one instance.
 * 
 * One pattern makes the exemplar and/or c'tor private and this 
 * class a friend of T_target.
 * 
 * If you just want to ensure an exemplar exists, use a template??. 
 * One time initialisation could then be done in the (private?) c'tor,
 * or the c'tor could do test ( this == exemplar )??.
 * 
 * usage
 * Use only:
 * class my_class { ...
 * friend class once_init< my_class >;
 * }
 * static once_init< my_class > my_class_dummy__;
 */
template<class T_once>
class once_init
{
  friend class friend_maker< T_once >::friend_type;
  private:
    int& counter();

    /**
     * Use this method to access the Singleton in your T_once class.
     */
    static T_once*& init()
    {
      static T_once* Result (0);
      return Result;
    }

  public:
    /**
     * This uses the empty ctor once to create the T_once Singleton.
     */
    once_init()
      {
        if (0 == counter ())
        {
          ++counter ();
          init () = new T_once;
        }
        ++counter ();
      }

    ~once_init() throw ()
      {
        --counter ();
        if (0 == counter ())
        {
          delete init ();
        }
      }


  private:
    /**
     * no copy
     */
    once_init(const once_init & source);

    /**
     * no assign
     */
    once_init & operator=(const once_init & source);


};
template<class T_once>
int& once_init<T_once>::counter() 
{
  static int Result (0);
  return Result;

}


} // namespace utility
#endif
