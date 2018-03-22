// ------------------------------------------------------------
// class: contract_error
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "utility/contract_error.hpp"

// Manual includes
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <tr1/memory>
// -
namespace utility {

/**
 * Create a contract exception with a formatted output. Primarily designed to 
 * operate with the Design-by-Contract macros. This class doesn't allocate 
 * memory from the heap.
 */
 contract_error::contract_error(const std::string & a_msg1, const char * a_msg2, unsigned int a_level, const char * a_filename, int a_line, const char * a_fn_name, void * const * a_backtrace, int a_backsz) throw () 
: std::exception ()
, level (a_level)
, m_what ()
{
  boost::iostreams::array_sink l_what (&m_what[0], s_size);
  boost::iostreams::stream< boost::iostreams::array_sink > l_errlog (l_what);
  l_errlog << "================== CONTRACT FAILURE ==================\n"
  << "REASON: \"" << a_msg1 << "\"\n"
  << "LEVEL[" << a_level << "] FAILED TEST: (" << a_msg2 << ")\n"
  << "FILE: " << a_filename << ", LINE: " << a_line << ".\n";
  if (NULL != a_fn_name)
  {
    l_errlog << "FUNCTION: " << a_fn_name << ".\n";
  }
#ifdef HAVE_BACKTRACE
  l_errlog << "BACKTRACE: \n";
  if (NULL != a_backtrace)
  {
    std::tr1::shared_ptr< char* > l_trace (backtrace_symbols (a_backtrace, a_backsz), &free);
    for (int l_i = 0; l_i < a_backsz; ++l_i)
    {
      l_errlog << *(l_trace.get () + l_i) << "\n";
    }
  }
#endif
  l_errlog << "================== CONTRACT FAILURE ==================\n";

}

contract_error::contract_error(const contract_error & a_other) 
: std::exception ()
, level (a_other.level)
, m_what ()
{
  std::char_traits< char >::copy (&m_what[0], &(a_other.m_what[0]), static_cast< size_t >(s_size));
}


contract_error & contract_error::operator=(const contract_error & a_other) 
{
  if (&a_other != this)
  {
    std::char_traits< char >::copy (&m_what[0], &(a_other.m_what[0]), s_size);
    const_cast< unsigned int& > (level) = a_other.level;
  }
  return *this;

}

/**
 * throw-less swap.
 */
void contract_error::swap(contract_error & a_other) 
{
  if (&a_other != this)
  {
    char l_what [s_size];
    std::char_traits< char >::copy (&l_what[0], &m_what[0], static_cast< size_t >(s_size));
    std::char_traits< char >::copy (&m_what[0], &(a_other.m_what[0]), static_cast< size_t >(s_size));
    std::char_traits< char >::copy (&(a_other.m_what[0]), &l_what[0], static_cast< size_t >(s_size));
    std::swap (const_cast< unsigned int& > (level), const_cast< unsigned int& > (a_other.level));
  }

}


} // namespace utility
