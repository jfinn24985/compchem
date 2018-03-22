// ------------------------------------------------------------
// class: supplier_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "supplier/supplier_helper.hpp"
#include "utility/program_options.hpp"

// Manual includes
#include <boost/bind.hpp>
// -
namespace supplier {

/**
 * The Singleton object.
 */
supplier_helper supplier_helper::s_exemplar;

/**
 * Constructor for singleton.
 */
supplier_helper::supplier_helper()
: m_plugin_dirs ()
, m_target_compound ()
, m_target_experiment ()
, m_target_file ()
, m_target_formula ()
, m_target_memo () 
, m_target_memo_src () 
{
  register_options ();
}

/**
 * Add options to the configuration helper.
 */
void supplier_helper::register_options() 
{
  // Add options to a_opt_set
  utility::program_options::helper ().public_options ().add_options ()
    ("experiment,e", boost::program_options::value< std::string > ()->notifier (boost::bind (&supplier_helper::target_experiment, this, _1)), "The target experiment for this application run.")
    ("file,i", boost::program_options::value< std::string > ()->notifier (boost::bind (&supplier_helper::target_file, this, _1)), "The target result file for this application run.")
    ("memo,m", boost::program_options::value< std::string > ()->notifier (boost::bind (&supplier_helper::target_memo, this, _1)), "The target memo for this application run.")
    ("memo-src,s", boost::program_options::value< std::string > ()->notifier (boost::bind (&supplier_helper::target_memo_src, this, _1)), "The location for memo-src for this application run.")
    ("filter-dir,F", boost::program_options::value< std::vector< std::string > > (&m_plugin_dirs)->composing (), "directory paths to search for filters (may be used multiple times)")
      ("compound,c", boost::program_options::value< std::string > ()->notifier (boost::bind (&supplier_helper::target_compound, this, _1)), "The target compound for the target memo (required for new memos)")
    ("formula,f", boost::program_options::value< std::string > ()->notifier (boost::bind (&supplier_helper::target_formula, this, _1)), "The target formula for the target memo (required for new memos)")
  ;

}


} // namespace supplier
