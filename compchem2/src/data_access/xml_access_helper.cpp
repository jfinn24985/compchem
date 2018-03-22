// ------------------------------------------------------------
// class: xml_access_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_access/xml_access_helper.hpp"
#include "utility/program_options.hpp"

// Manual includes in source
#include <deque>
#include <boost/bind.hpp>
// -
namespace data_access {

/**
 * The singleton helper object.
 */
xml_access_helper xml_access_helper::s_exemplar;

/**
 * Add a search directory. This is usually called during the program initialisation.
 */
void xml_access_helper::add_directory(const std::string& a_path) 
{
  m_dir_set.push_back (a_path);
}

/**
 * Add a set of search directories.
 */
void xml_access_helper::add_directories(const std::vector< std::string > & a_path) 
{
  m_dir_set.insert (m_dir_set.end (), a_path.begin (), a_path.end ());

}

/**
 * The default public ID for the system.
 */
std::string xml_access_helper::default_public_id()
{
  return std::string("http://www.purpletree.net.au/compchem/datascheme/V1.0.0/");
}

/**
 * The compile time defined system ID
 */
std::string xml_access_helper::default_system_id()
{
  return std::string("/home/finnerty/Office/Projects/compchem/schema/datascheme.dtd");
}

/**
 * Add options to the configuration helper.
 */
void xml_access_helper::register_options() 
{
  // Add options to the application option set.
  utility::program_options::helper ().public_options ().add_options ()
    ("autofix,A", boost::program_options::bool_switch ()->notifier (boost::bind (&xml_access_helper::auto_fix, this, true)), "automatically rebuild broken cross-references in database")
    ("search-dir,D", boost::program_options::value< std::vector< std::string > > ()->composing ()->notifier (boost::bind (&xml_access_helper::add_directories, this, _1)), "directory paths to search for broken cross-references (may be used multiple times)")
    ;
  utility::program_options::helper ().hidden_options ().add_options ()
    ("public-id", boost::program_options::value< std::string > (&m_public_id)->default_value (default_public_id ()), "the public ID to use for new XML documents.")
    ("system-id", boost::program_options::value< std::string > (&m_system_id)->default_value (default_system_id ()), "the system ID to use for new XML documents.")
    ;

}


} // namespace data_access
