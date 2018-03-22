// ------------------------------------------------------------
// class: filesystem
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "utility/filesystem.hpp"

// Manual includes
#include <boost/filesystem/operations.hpp>
// For compare
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
//-
namespace utility {

/**
 * Compare the contents of the file pointed to by a_path with
 * the contents buffer at a_start and length a_sz.
 */
bool filesystem::compare(const std::string& a_path, const void *const a_start, size_t a_sz)

{
  bool Result (false);
  // The file buffer is.
  int l_fd = open (a_path.c_str (), O_RDONLY);
  if (l_fd != -1)
  {
    struct stat l_sb; // To obtain file size
    if (fstat (l_fd, &l_sb) != -1)
    {
      if (a_sz == (size_t)l_sb.st_size)
      {
        void * l_start;
        l_start = mmap (NULL, l_sb.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, l_fd, 0);
        if (l_start != MAP_FAILED)
        {
          Result = (0 == memcmp (l_start, a_start, l_sb.st_size));
          if (not Result)
          {
             DLOG ("Buffer and file ["); DLOG(a_path); DLOG ("] did not compare equal.\n");
          }
        }
        else
        {
          DLOG ("Mapping file ["); DLOG(a_path); DLOG ("] failed.\n");
        }
      }
      else
      {
        DLOG("Different sizes, file ["); DLOG(a_path); DLOG ("] = "); DLOG(l_sb.st_size); DLOG(", buffer = "); DLOG (a_sz); DLOG ("\n");
      }
    }
    else
    {
      DLOG ("Unable to stat file ["); DLOG(a_path); DLOG ("].\n");
    }
    close (l_fd);
  }
  else
  {
    DLOG ("Unable to open file ["); DLOG(a_path); DLOG ("] \n");
  }
  return Result;

}

/**
 * Standardise a_path to be complete and normalized.
 */
boost::filesystem::path filesystem::standardize(const boost::filesystem::path & a_path)
{
  boost::filesystem::path Result (a_path);
  if (not Result.is_complete ())
  {
    Result = boost::filesystem::initial_path () / Result;
  }
  Result.normalize ();
  return Result;
}

/**
 * Attempt to create a relative path from a_base to a_leafer.  If this 
 * is not possible a_leafer is returned unaltered.  The relative path is
 * constructed relative to the whole of a_base.  If either paths are
 * not complete, then they are made "complete" by prepending initial_path ()
 * and normalizing.
 */
boost::filesystem::path filesystem::relative_path(const boost::filesystem::path & a_base, const boost::filesystem::path & a_leafer)
{
  // Ensure that starting paths are complete
  boost::filesystem::path l_base (standardize (a_base));
  boost::filesystem::path l_leaf (standardize (a_leafer));
  boost::filesystem::path Result;
  boost::filesystem::path::iterator l_lhs (l_base.begin ());
  boost::filesystem::path::iterator l_rhs (l_leaf.begin ());
  // Iterate along a_base and a_leafer for first mismatch.
  if (*l_lhs != *l_rhs)
  { // No common base, just return a_leafer
    Result = l_leaf;
  }
  else
  {
    for (++l_rhs, ++l_lhs; // Iterate to first mismatch.
  	l_lhs != l_base.end () and l_rhs != l_leaf.end () and (*l_lhs) == (*l_rhs);
  	++l_rhs, ++l_lhs); // Do nothing in loop.
    if (l_lhs != l_base.end ())
    { // Need to add "parent dirs" until we reach end
      for (; l_lhs != l_base.end (); ++l_lhs)
      {
        Result /= "..";
      }
    }
    if (l_lhs == l_base.end ())
    { // Now everything from l_rhs is in path.
      for (; l_rhs != l_leaf.end (); ++l_rhs)
      {
        Result /= *l_rhs;
      }
    }
  }
  return Result;
}


} // namespace utility
