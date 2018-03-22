#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

// ------------------------------------------------------------
// class: filesystem
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>



namespace utility {

/**
 * A set of utility functions for operating on filesystem objects.
 */
class filesystem {
  public:
    /**
     * Compare the contents of the file pointed to by a_path with
     * the contents buffer at a_start and length a_sz.
     */
    static bool compare(const std::string& a_path, const void *const a_start, size_t a_sz);

    /**
     * Standardise a_path to be complete and normalized.
     */
    static boost::filesystem::path standardize(const boost::filesystem::path & a_path);

    /**
     * Attempt to create a relative path from a_base to a_leafer.  If this 
     * is not possible a_leafer is returned unaltered.  The relative path is
     * constructed relative to the whole of a_base.  If either paths are
     * not complete, then they are made "complete" by prepending initial_path ()
     * and normalizing.
     */
    static boost::filesystem::path relative_path(const boost::filesystem::path & a_base, const boost::filesystem::path & a_leafer);

};

} // namespace utility
#endif
