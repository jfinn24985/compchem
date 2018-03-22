#ifndef COMPCHEM_SUPPLIER_ERROR_HPP
#define COMPCHEM_SUPPLIER_ERROR_HPP

// ------------------------------------------------------------
// class: supplier_error
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <stdexcept>

#include <string>

namespace supplier {

/**
 * Exception class to report filter failures.
 */
class supplier_error : public std::logic_error
{
  public:
    /**
     * The error subtype. This should be one of the result_values:
     * ON_ERROR, ON_LOADERROR, ON_NOFILE, ON_BADDATA
     * or ON_BADCONFIG
     */
    const int level;

    /**
     * Create an error
     */
    supplier_error(const std::string& a_msg, unsigned int a_val)
    : std::logic_error (a_msg)
    , level (a_val)
    {}

    virtual ~supplier_error() throw()
    {}


};

} // namespace supplier
#endif
