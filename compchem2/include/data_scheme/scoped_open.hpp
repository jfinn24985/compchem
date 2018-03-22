#ifndef COMPCHEM_DATA_SCHEME_SCOPED_OPEN_HPP
#define COMPCHEM_DATA_SCHEME_SCOPED_OPEN_HPP

// ------------------------------------------------------------
// class: scoped_open
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
namespace data_scheme {

/**
 * Class to automatically open/close a src type object.
 */
template<class t_src_type>
class scoped_open
{
  public:
    typedef t_src_type value_type;


  private:
    /**
     * The object to open/close.
     */
    value_type * m_obj;


  public:
    scoped_open(value_type & a_obj)
    : m_obj (a_obj.is_src () and not a_obj.is_open ()? &a_obj : NULL)
    {
      if (NULL != m_obj) m_obj->read ();
    }

    /**
     * Break constness when necessary in a controlled manner.
     */
    scoped_open(const value_type & a_obj)
    : m_obj (a_obj.is_src () and not a_obj.is_open ()? const_cast< value_type* >(&a_obj) : NULL)
    {
      if (NULL != m_obj) m_obj->read ();
    }

    ~scoped_open()
    {
      if (NULL != m_obj) m_obj->close ();
    }


  private:
    /**
     * no copy
     */
    scoped_open(const scoped_open<t_src_type> &);

    /**
     * no assign
     */
    scoped_open<t_src_type> & operator=(const scoped_open<t_src_type> &);


};

} // namespace data_scheme
#endif
