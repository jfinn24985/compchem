// ------------------------------------------------------------
// class: time_stamp
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "utility/time_stamp.hpp"

namespace utility {

/**
 * Copy a_msg to the log stream along with the current timer values.
 */
void time_stamp::log(const std::string& a_msg) 
{
  // Print message
  std::streamsize l_prec (m_os.precision (6));
  m_os << "[" << m_label << "]TS: e(" << m_last.elapsed ();
  m_os << ") t(" << m_start.elapsed () << ") " << a_msg << "\n";
  m_last.restart ();
  m_os.precision (l_prec);
  

}

/**
 * Initialise a time stamp object with a_label.  Copy a_start_msg to a_os.
 */
time_stamp::time_stamp(const std::string& a_label, const std::string& a_start_msg, std::ostream & a_os) 
: m_label (a_label)
, m_last ()
, m_os (a_os)
, m_start (m_last)
{
  m_os << "[" << m_label << "]TS: " << a_start_msg << "\n";
} 


time_stamp::~time_stamp() 
{
  log ("End of timer.");
}


} // namespace utility
