// ------------------------------------------------------------
// class: authentication_manager
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "utility/authentication_manager.hpp"

namespace utility {

/**
 * The service name for this application.  This is the name the authentication manager will use to identify the application/service to the external authentication system.  The external system will need to be configured to recognise this name.
 */
std::string authentication_manager::service_name;

/**
 * Set the service name
 */
void authentication_manager::set_service_name(const std::string value)
{
  service_name = value;
}


} // namespace utility
