#ifndef AUTHENTICATION_MANAGER_HPP
#define AUTHENTICATION_MANAGER_HPP

// ------------------------------------------------------------
// class: authentication_manager
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include <memory>

namespace utility {

/**
 * Abstract base class for authentication managers.
 */
class authentication_manager {
  public:
    /**
     * The service name for this application.  This is the name the authentication manager will use to identify the application/service to the external authentication system.  The external system will need to be configured to recognise this name.
     */
    static std::string service_name;

    /**
     * Create an authentication manager object for the given user. (To be defined in the source of each authentication sub-class, only one sub-class can be linked into application.)
     */
    static std::auto_ptr< authentication_manager > create_manager(const std::string& a_user, const std::string& a_service);

    /**
     * A textual message for why the user is not authenticated.
     * 
     * \pre !is_valid && is_valid_connection
     */
    virtual std::string error_string() const = 0;

    /**
     * Get the service name 
     */
    static inline const std::string get_service_name();

    /**
     * Get the current value of 'user'. (cached)
     */
    virtual std::string get_user() const = 0;

    /**
     * Is the current user authenticated by the system.  If 'false' the reason for 
     * the error can be found in error_number (and error_string).
     */
    virtual bool is_authenticated() const = 0;

    /**
     * Does the object have an error?
     */
    virtual bool is_valid() const = 0;

    /**
     * Set the service name
     */
    static void set_service_name(const std::string value);

    virtual ~authentication_manager() {}

};
/**
 * Get the service name 
 */
inline const std::string authentication_manager::get_service_name()
{
  return service_name;
}


} // namespace utility
#endif
