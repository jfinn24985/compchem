// ------------------------------------------------------------
// class: data_scheme_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/data_scheme_helper.hpp"
#include "utility/program_options.hpp"

// Manual includes in source
#include <boost/bind.hpp>
// -
namespace data_scheme {

/**
 * The Singleton object.
 */
data_scheme_helper data_scheme_helper::s_exemplar;

/**
 * Constructor for singleton.
 */
data_scheme_helper::data_scheme_helper()
: m_auto_memo (false)
, m_auto_project (false)
, m_target_project ("default") 
{
  register_options ();
}

/**
 * Add options to the configuration helper.
 */
void data_scheme_helper::register_options() 
{
  // Add options to a_opt_set
  utility::program_options::helper ().public_options ().add_options ()
    ("project,p", boost::program_options::value< std::string > ()->notifier (boost::bind (&data_scheme_helper::target_project, this, _1)), "The target project for this application run.")
    ("auto-open-memo,M", boost::program_options::bool_switch (&(this->m_auto_memo))
          , "Normally the application will not open memo-src subfiles while reading the parent file. This option causes the application to read the memo-src objects automatically.")
    ("auto-project,P", boost::program_options::bool_switch (&(this->m_auto_project))
          , "Normally the application will exit when a target project is not present. This option causes the application to attempt to create a new project instead.");

}


} // namespace data_scheme
