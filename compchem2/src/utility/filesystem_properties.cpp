// ------------------------------------------------------------
// class: filesystem_properties
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "utility/filesystem_properties.hpp"

// Manual includes
// --
#ifdef HAVE_GETCWD_EXT
#include <tr1/memory>
#endif
#include <boost/scoped_array.hpp>
// --
namespace utility {

/**
 * Get working directory name
 * 
 * \post Result may be an empty string.
 * \return An empty string indicates failure.
 */
std::string filesystem_properties::current_directory()
{
  // LOCAL
  std::string Result;
  // DO
#ifdef HAVE_GETCWD_EXT
  {
    std::tr1::shared_ptr< char > l_buffer (getcwd (NULL, 0), &free);
    Result.assign (l_buffer.get ());
  }
#else
  {
    size_t l_size = 256;
    int l_errno;
    do
    {
      boost::scoped_array< char > l_buffer(new char [l_size]);
      errno = 0;// reset error indicator.
      if (l_buffer.get () == getcwd (l_buffer.get (), l_size - 1))
      {
        // Assign string on success.
        l_buffer[l_size - 1] = '\0'; // Ensure is nul terminated
        Result.assign (l_buffer.get ());
        l_errno = 0;
      }
      else
      {
        l_errno = errno;
        // Double size (in case we loop on ERANGE)
        l_size *= 2;
      }
    }
    while (l_errno == ERANGE);
  }
#endif
  // END
  return Result;
  
}

/**
 * Get effective user's home directory name
 * 
 * \post Return of empty string indicates an error.
 */
std::string filesystem_properties::home_directory()
{
  // LOCAL
  size_t l_size = 1024;
  int l_libc_result = 0;
  std::string Result;
  // DO
  do
  {
    boost::scoped_array< char > l_buffer(new char [l_size]);
    struct passwd l_datum;
    struct passwd* l_datum_ptr;
    l_libc_result = getpwuid_r (effective_user ()
                              , &l_datum
                              , l_buffer.get ()
                              , l_size
                              , &l_datum_ptr);
    // Copy if function call succeeded
    if (0 == l_libc_result && &l_datum == l_datum_ptr)
    {
      Result.assign (l_datum.pw_dir);
    }
    // Double buffer size in case we loop on ERANGE
    l_size *= 2;
  }
  while (l_libc_result == ERANGE);
  // END
  return Result;
  
}

/**
 * Swap state of this with a_other.
 * 
 * strong throw guaranttee
 */
void filesystem_properties::swap(filesystem_properties & a_other) 
{
  std::swap (m_stat, a_other.m_stat);
}


} // namespace utility
