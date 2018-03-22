#ifndef COMPCHEM_TIME_STAMP_HPP
#define COMPCHEM_TIME_STAMP_HPP

// ------------------------------------------------------------
// class: time_stamp
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include <boost/timer.hpp>


namespace utility {

/**
 * This class allows monitoring of the progress of an application. At construction
 * it prints a message and starts recording the time. Every call to log prints
 * the total elapsed time and the elapsed time since the last call to log.
 */
class time_stamp
{
  private:
    /**
     * The time stamp's label.
     */
    std::string m_label;

    /**
     * The last time a time_stamp output function was called.
     */
    boost::timer m_last;

    /**
     * The output stream to use.
     */
    std::ostream & m_os;

    /**
     * The total elapsed time.
     */
    boost::timer m_start;


  public:
    /**
     * Copy a_msg to the log stream along with the current timer values.
     */
    void log(const std::string& a_msg);

    /**
     * Initialise a time stamp object with a_label.  Copy a_start_msg to a_os.
     */
    time_stamp(const std::string& a_label, const std::string& a_start_msg, std::ostream & a_os);

    ~time_stamp();


  private:
    /**
     * no copy
     */
    time_stamp(const time_stamp & source);

    /**
     * no assign
     */
    time_stamp & operator=(const time_stamp & source);


};

} // namespace utility
#endif
