#ifndef EPOCH_TIME_HPP
#define EPOCH_TIME_HPP

// ------------------------------------------------------------
// class: epoch_time
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
// C++ standard C library header for time functions.
#include <ctime>
namespace utility {

/**
 * A time value as represented by the C time_t value.
 */
class epoch_time {
  public:
    /**
     * Is earlier date
     */
    bool is_earlier(const epoch_time & a_day) const throw()
    {
      return m_epoch_time < a_day.m_epoch_time;
    }

    /**
     * Is same date
     */
    bool is_same(const epoch_time & a_day) const throw()
    {
      return m_epoch_time == a_day.m_epoch_time;
    }

    /**
     * Conversion to string operator.  This can be used to convert epoch_time to a Boost date_time.
     * 
     * \throw COMPCHEM_ALWAYS, additionally std::string constructor can throw.
     */
    inline  operator std::string() const;

    /**
     * Default constructor makes a date time that represents now.
     */
    epoch_time()
    : m_epoch_time (time(NULL))
    {
      COMPCHEM_ALWAYS (m_epoch_time != -1, "Unable to get system time");
    }
    /**
     * Construct time from a POSIC time_t object.
     */
    epoch_time(time_t a_time) throw()
    : m_epoch_time (a_time)
    {}

    /**
     * Copy constructor 
     */
    epoch_time(const epoch_time & a_day) throw()
    : m_epoch_time (a_day.m_epoch_time)
    {}

    /**
     * Get the POSIX time_t value
     */
    const time_t get_epoch_time() const
    {
      return m_epoch_time;
    }

    /**
     * Copy operator
     */
    const epoch_time & operator =(const epoch_time & a_day) throw()
    {
      m_epoch_time = a_day.m_epoch_time;
      return *this;
    }

    friend bool operator>(const epoch_time & lhs, const epoch_time & rhs)
    {
      return lhs.m_epoch_time > rhs.m_epoch_time;
    }

    friend bool operator>=(const epoch_time & lhs, const epoch_time & rhs)
    {
      return lhs.m_epoch_time >= rhs.m_epoch_time;
    }

    friend bool operator<(const epoch_time & lhs, const epoch_time & rhs)
    {
      return lhs.m_epoch_time < rhs.m_epoch_time;
    }

    friend bool operator<=(const epoch_time & lhs, const epoch_time & rhs)
    {
      return lhs.m_epoch_time <= rhs.m_epoch_time;
    }

    friend bool operator==(const epoch_time & lhs, const epoch_time & rhs)
    {
      return lhs.m_epoch_time == rhs.m_epoch_time;
    }

    friend bool operator!=(const epoch_time & lhs, const epoch_time & rhs)
    {
      return lhs.m_epoch_time != rhs.m_epoch_time;
    }

    /**
     * Destructor
     */
    ~epoch_time() throw()
    {}


  private:
    time_t m_epoch_time;

};
/**
 * Conversion to string operator.  This can be used to convert epoch_time to a Boost date_time.
 * 
 * \throw COMPCHEM_ALWAYS, additionally std::string constructor can throw.
 */
inline  epoch_time::operator std::string() const 
{
  char* l_result;
#ifdef REENTRANT
  char l_buff[64];
  l_result = ctime_r (&m_epoch_time, &l_buff[0]);
#else
  l_result = ctime (&m_epoch_time);
#endif
  COMPCHEM_ALWAYS (NULL != l_result, "Unable to convert time value");
  return std::string(l_result);

}


} // namespace utility
#endif
