#ifndef PAM_MANAGER_HPP
#define PAM_MANAGER_HPP

// ------------------------------------------------------------
// class: pam_manager
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include <security/pam_appl.h>

#include "utility/authentication_manager.hpp"

namespace utility {

/**
 * Manage user/worker authentication 
 */
class pam_manager : public authentication_manager {
  private:
    /**
     * The cached name of the current user.  This may be changed by the authentication system to something other than the initial value.
     */
    std::string m_user;

    pam_conv m_pam_conversation;

    pam_handle_t * m_pam_handle;

    /**
     * The current error number.
     */
    mutable int m_error_number;

    /**
     * Static function used as callback by PAM library. (TODO)
     * 
     * \param a_num_msg The size of the message array.
     * \param a_pam_message A query for information from the user (array size is a_num_msg).
     * \param a_pam_response A array in which to place the results of the request (array size is a_num_msg).
     */
    static int conversation_function(int a_num_msg, const pam_message ** a_pam_message, pam_response ** a_pam_response, void * a_object);


  public:
    /**
     * A code number for the reason the user is not authenticated.
     */
    int error_number() const { return m_error_number; }

    /**
     * A textual message for why the user is not authenticated.
     * 
     * \pre !is_valid && is_valid_connection
     */
    virtual std::string error_string() const;

    /**
     * Query what the requesting user host.
     * 
     * \pre is_valid
     */
    std::string get_rhost() const;

    /**
     * Query what the requesting user name is.
     * 
     * \pre is_valid
     */
    std::string get_ruser() const;

    /**
     * Get the current value of 'user'. (cached)
     */
    virtual std::string get_user() const { return m_user; }

    /**
     * Query what the current user prompt is.
     * 
     * \pre is_valid
     */
    std::string get_user_prompt() const;


  private:
    /**
     * Query what the current value of 'user' from the external system.
     * 
     * \pre is_valid
     */
    void get_user_priv();


  public:
    /**
     * Is the current user authenticated by the system.  If 'false' the reason for 
     * the error can be found in error_number (and error_string).
     */
    virtual bool is_authenticated() const;

    /**
     * Does the object have an error?
     */
    virtual bool is_valid() const { return m_error_number == NO_ERROR; }

    /**
     * Is there a valid connection to the external system? This will only be false if is_valid is false.
     * 
     * \post is_valid <-> result == true
     * \post false --> !is_valid
     */
    bool is_valid_connection() const { return m_error_number != INVALID_HANDLE; }

    /**
     * Create a PAM session for the given user.
     * 
     * \pre a_name.empty () /= true
     */
    pam_manager(const std::string& a_name);

    virtual ~pam_manager();

    /**
     * Set the requesting host name.
     * 
     * \pre is_valid
     */
    void set_rhost(const std::string& a_host);

    /**
     * Set the requesting user.
     * 
     * \pre is_valid
     */
    void set_ruser(const std::string& a_user);


  private:
    /**
     * Set what the current user prompt is.
     * 
     * \pre is_valid
     */
    void set_user_priv(const std::string& a_user);


  public:
    /**
     * Set what the current user prompt is.
     * 
     * \pre is_valid
     */
    void set_user_prompt(const std::string& a_prompt);


  private:
    /**
     * Undefined
     */
    pam_manager(const pam_manager & source);

    /**
     * Undefined
     */
    pam_manager & operator=(const pam_manager & source);


  public:
    /**
     * Indicate that the system is not in an error state.
     */
    static const int NO_ERROR;


  private:
    /**
     * Indicate authentication cannot proceed because connection to external authentication system has not been established. 
     * 
     * * User should never get this error as it means a function precondition failed to detect the error bad connection condition.
     */
    static const int INVALID_HANDLE;

    /**
     * An attempt to do something invalid such as replacing the conversation structure with a NULL value.
     * 
     * * The user should never get this error.  The class interface precludes this operation.
     */
    static const int PERMISSION_DENIED;


  public:
    /**
     * Ran out of memory during some operation.
     */
    static const int MEMORY_ERROR;


  private:
    /**
     * The application attempted to set an undefined or inaccessible item.
     * 
     * * The user should never see this error as class interface precludes it.
     */
    static const int INVALID_OPERATION;

    /**
     * The user was not authenticated.
     * 
     * * The user should never get this error. Function guard conditions should protect functions from this condition.
     */
    static const int AUTHENTICATION_ERROR;


  public:
    /**
     * For some reason the application does not have sufficient credentials to authenticate the user.
     */
    static const int INSUFFICIENT_RIGHTS;

    /**
     * The modules were not able to access the authentication information. This might be due to a network or hardware failure etc.
     */
    static const int CANNOT_AUTHENTICATE;

    /**
     * The supplied username is not known to the authentication service.
     */
    static const int USER_UNKNOWN;

    /**
     * One or more of the authentication modules has reached its limit of tries authenticating the user. Do not try again.
     */
    static const int MAX_TRIES;

    /**
     * One or more of the authentication modules failed to load.
     */
    static const int SYSTEM_ERROR;

};

} // namespace utility
#endif
