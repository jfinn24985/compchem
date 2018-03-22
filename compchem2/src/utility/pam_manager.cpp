// ------------------------------------------------------------
// class: pam_manager
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "utility/pam_manager.hpp"

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <boost/scoped_array.hpp>
namespace utility {

/**
 * Static function used as callback by PAM library. (TODO)
 * 
 * \param a_num_msg The size of the message array.
 * \param a_pam_message A query for information from the user (array size is a_num_msg).
 * \param a_pam_response A array in which to place the results of the request (array size is a_num_msg).
 */
int pam_manager::conversation_function(int a_num_msg, const pam_message ** a_pam_message, pam_response ** a_pam_response, void * a_object)
{
  // local
  // do
  LOG("Conversation function")
  DLOG("Called with ");DLOG(a_num_msg);DLOG(" messages.\n");
  // Ignore calling object (at the moment)..
  
  // Ask all questions
  for (int l_i = 0; l_i < a_num_msg; ++l_i)
  {
    // Current message is "a_pam_message[l_i]"
    DLOG("Current message is a_pam_message[l_i]\n");
    std::cout << a_pam_message[l_i]->msg;
    switch (a_pam_message[l_i]->msg_style)
    {
    case PAM_PROMPT_ECHO_OFF:  // Fall thru
  	DLOG("PAM Prompt echo off\n");
    case PAM_PROMPT_ECHO_ON:
  	{
  	DLOG("PAM Prompt echo on\n");
  	boost::scoped_array< char > l_result (new char [256]);
   	std::cin.getline (l_result.get (), 256);
  	DLOG ("Got result for PAM\n");
  	DLOG ("Input = [");DLOG(l_result.get ());DLOG("]\n");
  	// Ignore over-length lines.
  	if (NULL == a_pam_response[l_i])
  	{
  		a_pam_response[l_i] = (struct pam_response*)malloc (sizeof (struct pam_response));
  	}
  	COMPCHEM_ALWAYS(NULL != a_pam_response[l_i], "C memory allocation failed");
  	a_pam_response[l_i]->resp = strdup(l_result.get ());
  	a_pam_response[l_i]->resp_retcode = 0;
  	DLOG("PAM response message set\n");
  	}
  	break;
     case PAM_ERROR_MSG: // fall thru
  	DLOG("PAM Error message\n");
     case PAM_TEXT_INFO: // fall thru
  	DLOG("PAM info\n");
     default:
  	// do nothing
  	break;
     }
  }
  
  return PAM_SUCCESS;
}

/**
 * A textual message for why the user is not authenticated.
 * 
 * \pre !is_valid && is_valid_connection
 */
std::string pam_manager::error_string() const {
  // local
  std::string Result;
  // Check valid handle
  REQUIRE(is_valid () == false, "Must be in error state to get an error message");
  // Use PAM!!
  if (is_valid_connection () == false)
  {
  	Result = "No connection to authentication server.";
  } else {
  	Result = pam_strerror (m_pam_handle, m_error_number);
  }
  return Result;
}

/**
 * Query what the requesting user host.
 * 
 * \pre is_valid
 */
std::string pam_manager::get_rhost() const {
  // local
  const char *l_result;
  std::string result;
  // PRE
  REQUIRE(is_valid (), "Valid object required.");
  // do
  m_error_number = pam_get_item (m_pam_handle, PAM_RHOST, reinterpret_cast <const void**> (&l_result));
  if (NO_ERROR == m_error_number)
  {
   result = l_result;
  }
  return result;
}

/**
 * Query what the requesting user name is.
 * 
 * \pre is_valid
 */
std::string pam_manager::get_ruser() const {
  // local
  const char *l_result;
  std::string result;
  // PRE
  REQUIRE(is_valid (), "Valid object required.");
  // do
  m_error_number = pam_get_item (m_pam_handle, PAM_RUSER, reinterpret_cast <const void**> (&l_result));
  if (NO_ERROR == m_error_number)
  {
   result = l_result;
  }
  return result;
}

/**
 * Query what the current user prompt is.
 * 
 * \pre is_valid
 */
std::string pam_manager::get_user_prompt() const {
  // local
  const char *l_result;
  std::string result;
  // PRE
  REQUIRE(is_valid (), "Valid object required.");
  // do
  m_error_number = pam_get_item (m_pam_handle, PAM_USER_PROMPT, reinterpret_cast <const void**> (&l_result));
  if (NO_ERROR == m_error_number)
  {
   result = l_result;
  }
  return result;
}

/**
 * Query what the current value of 'user' from the external system.
 * 
 * \pre is_valid
 */
void pam_manager::get_user_priv() {
  // local
  const void *l_item;
  // PRE
  REQUIRE(is_valid (), "Valid object required.");
  // do
  m_error_number = pam_get_item (m_pam_handle, PAM_USER, &l_item);
  if (NO_ERROR == m_error_number)
  {
    m_user = static_cast<const char*>(l_item);
    std::cout << "Current name for user with PAM is [" << m_user << "]" << std::endl;
  }
}

/**
 * Is the current user authenticated by the system.  If 'false' the reason for 
 * the error can be found in error_number (and error_string).
 */
bool pam_manager::is_authenticated() const {
  return is_valid ();
}

/**
 * Create a PAM session for the given user.
 * 
 * \pre a_name.empty () /= true
 */
pam_manager::pam_manager(const std::string& a_name) :
m_user(a_name)
, m_pam_conversation()
, m_pam_handle(NULL)
, m_error_number(NO_ERROR)
 {
  // local
  typedef int (*pam_conv_fn)(int, const pam_message**, pam_response**, void*);
  // PRECOND
  REQUIRE (a_name.empty () != true, "Cannot authenticate user with no name");
  // Set up m_pam_conversation
  m_pam_conversation.conv =  static_cast<pam_conv_fn> (&pam_manager::conversation_function);
  m_pam_conversation.appdata_ptr = reinterpret_cast< void* > (this);
  // Log
  DLOG("Authenticating user [");DLOG(m_user);DLOG("]\n");
  // Something like
  m_error_number = pam_start (get_service_name().c_str ()
                             , m_user.c_str ()
                             , &m_pam_conversation 
                             , &m_pam_handle);
  // Authenticate user
  if (NO_ERROR != m_error_number) 
  {
    std::cout << "Authentication service not available" << std::endl;
  }
  else
  {
    // Get the user (and write name to log)
    get_user_priv ();
    LOG(get_user ().c_str ());
    // Attempt authentication
    m_error_number = pam_authenticate (m_pam_handle, 0);
  
    // Account management settings
    if (NO_ERROR != m_error_number)
    {
      // Authentication failed, write string to output
      std::cout << "Unable to Authenticate user: ";
      std::cout << error_string () << std::endl;
    }
    else
    {
      // Get user again (in case authentication changed the user.)
      get_user_priv ();
      LOG(get_user ().c_str ());
      // Try to set the user name we know.
      set_user_priv (a_name);
      LOG(get_user ().c_str ());
      
      // Call account management function
      m_error_number = pam_acct_mgmt (m_pam_handle, 0);
  
      // Set any credentials
      if (NO_ERROR != m_error_number) 
      {
        // Account management failed
        std::cout << "Authentication account management failed: ";
        std::cout << error_string () << std::endl;
      }
      else
      {
        // Check user name again
        get_user_priv ();
        LOG(get_user ().c_str ());
        
        // Call credential function.
        m_error_number = pam_setcred (m_pam_handle, PAM_ESTABLISH_CRED);
  
        // Start a session
        if (NO_ERROR != m_error_number)
        {
  	// Setting credentials failed.
  	std::cout << "Setting Authentication credentials failed: ";
  	std::cout << error_string () << std::endl;
        }
        else
        {
  	// Get the user name (which may have changed)
  	get_user_priv ();
  	LOG(get_user ().c_str ());
  	
  	// Start a session
  	m_error_number = pam_open_session (m_pam_handle, 0);
  
  	if (NO_ERROR != m_error_number) 
  	{
  	  std::cout << "Openning Authentication session failed: ";
  	  std::cout << error_string () << std::endl;
  	}
  	else
  	{
  	  // Get the user name (which may have changed)
  	  get_user_priv ();
  	  LOG(get_user ().c_str ());
  	}
        }
      }
    }
  }
}

pam_manager::~pam_manager() {
  // Test m_pam_handle
  if (NULL != m_pam_handle)
  {
  // Something like:
  m_error_number = pam_close_session (m_pam_handle, 0);
  //TODO Do something with return value??
  
  m_error_number = pam_end (m_pam_handle
                            , m_error_number);
  //TODO Do something with return value??
  
  }
}

/**
 * Set the requesting host name.
 * 
 * \pre is_valid
 */
void pam_manager::set_rhost(const std::string& a_host) {
  // local
  // PRE
  REQUIRE(is_valid (), "Valid object required.");
  // do
  m_error_number = pam_set_item (m_pam_handle, PAM_RHOST, reinterpret_cast <const void*> (a_host.c_str ()));
}

/**
 * Set the requesting user.
 * 
 * \pre is_valid
 */
void pam_manager::set_ruser(const std::string& a_user) {
  // local
  // PRE
  REQUIRE(is_valid (), "Valid object required.");
  // do
  m_error_number = pam_set_item (m_pam_handle, PAM_RUSER, reinterpret_cast <const void*> (a_user.c_str ()));
}

/**
 * Set what the current user prompt is.
 * 
 * \pre is_valid
 */
void pam_manager::set_user_priv(const std::string& a_user) {
  // local
  // PRE
  REQUIRE(is_valid (), "Valid object required.");
  // do
  m_error_number = pam_set_item (m_pam_handle, PAM_USER, static_cast <const void*> (a_user.c_str ()));
  m_user = a_user;
}

/**
 * Set what the current user prompt is.
 * 
 * \pre is_valid
 */
void pam_manager::set_user_prompt(const std::string& a_prompt) {
  // local
  // PRE
  REQUIRE(is_valid (), "Valid object required.");
  // do
  m_error_number = pam_set_item (m_pam_handle, PAM_USER_PROMPT, reinterpret_cast <const void*> (a_prompt.c_str ()));
}

/**
 * Indicate that the system is not in an error state.
 */
const int pam_manager::NO_ERROR(PAM_SUCCESS);

/**
 * Indicate authentication cannot proceed because connection to external authentication system has not been established. 
 * 
 * * User should never get this error as it means a function precondition failed to detect the error bad connection condition.
 */
const int pam_manager::INVALID_HANDLE(PAM_SYSTEM_ERR);

/**
 * An attempt to do something invalid such as replacing the conversation structure with a NULL value.
 * 
 * * The user should never get this error.  The class interface precludes this operation.
 */
const int pam_manager::PERMISSION_DENIED(PAM_PERM_DENIED);

/**
 * Ran out of memory during some operation.
 */
const int pam_manager::MEMORY_ERROR(PAM_BUF_ERR);

/**
 * The application attempted to set an undefined or inaccessible item.
 * 
 * * The user should never see this error as class interface precludes it.
 */
const int pam_manager::INVALID_OPERATION(PAM_BAD_ITEM);

/**
 * The user was not authenticated.
 * 
 * * The user should never get this error. Function guard conditions should protect functions from this condition.
 */
const int pam_manager::AUTHENTICATION_ERROR(PAM_AUTH_ERR);

/**
 * For some reason the application does not have sufficient credentials to authenticate the user.
 */
const int pam_manager::INSUFFICIENT_RIGHTS(PAM_CRED_INSUFFICIENT);

/**
 * The modules were not able to access the authentication information. This might be due to a network or hardware failure etc.
 */
const int pam_manager::CANNOT_AUTHENTICATE(PAM_AUTHINFO_UNAVAIL);

/**
 * The supplied username is not known to the authentication service.
 */
const int pam_manager::USER_UNKNOWN(PAM_USER_UNKNOWN);

/**
 * One or more of the authentication modules has reached its limit of tries authenticating the user. Do not try again.
 */
const int pam_manager::MAX_TRIES(PAM_MAXTRIES);

/**
 * One or more of the authentication modules failed to load.
 */
const int pam_manager::SYSTEM_ERROR(PAM_ABORT);


// Create an authentication manager object for the given user.
std::auto_ptr< authentication_manager > authentication_manager::create_manager(const std::string& a_user, const std::string& a_service)
{
  REQUIRE (not a_service.empty () or
	not get_service_name ().empty (), "Cannot use empty string for service name.");
  if (not a_service.empty ())
  {
    set_service_name (a_service);
  }
  return std::auto_ptr< authentication_manager >(new pam_manager (a_user));
}


} // namespace utility
