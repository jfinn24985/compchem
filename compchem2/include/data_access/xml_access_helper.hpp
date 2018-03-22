#ifndef COMPCHEM_XML_ACCESS_HELPER_HPP
#define COMPCHEM_XML_ACCESS_HELPER_HPP

// ------------------------------------------------------------
// class: xml_access_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


#include <string>

namespace utility { class program_options; } 

namespace data_access {

/**
 * This contains functions to attempt to registers program options for controlling 
 * the operation of the xml_persistence broken-link file recovery seach.
 * 
 * Set options:
 *  autofix,A: automatically rebuild broken cross-references in database
 *  search-dir,D: directory paths to search for broken cross-references (may be used multiple times)
 *  (hidden options)
 *  public-id: the public ID to use for new XML documents.
 *  system-id: the system ID to use for new XML documents.
 */
class xml_access_helper {
  public:
    typedef std::vector< boost::filesystem::path >::const_iterator const_iterator;


  private:
    /**
     * Whether the system should rebuild broken links automatically.
     */
    bool m_auto_fix;

    /**
     * The set of directory paths to search.
     */
    std::vector< boost::filesystem::path > m_dir_set;

    /**
     * The public ID of the current program version.
     */
    std::string m_public_id;

    /**
     * The system ID, this is dependent on the installation.
     */
    std::string m_system_id;

    /**
     * The singleton helper object.
     */
    static xml_access_helper s_exemplar;


  public:
    /**
     * Add a search directory. This is usually called during the program initialisation.
     */
    void add_directory(const std::string& a_path);

    /**
     * Add a set of search directories.
     */
    void add_directories(const std::vector< std::string > & a_path);

    /**
     * Should we automatically rebuild links?
     */
    bool auto_fix() const throw ()
    {
      return m_auto_fix;
    }

    /**
     * Set if we automatically rebuild links or not.
     */
    void auto_fix(bool a_fix) throw ()
    {
      m_auto_fix = a_fix;
    }

    /**
     * Get a const iterator to the first search dir.
     */
    const_iterator begin() const
    {
      return m_dir_set.begin ();
    }

    /**
     * The default public ID for the system.
     */
    static std::string default_public_id();

    /**
     * The compile time defined system ID
     */
    static std::string default_system_id();

    /**
     * Are there any registered search dirs?
     */
    bool empty() const throw ()
    {
      return m_dir_set.empty ();
    }

    /**
     * Get const iterator to the one-past-last search dir.
     */
    const_iterator end() const
    {
      return m_dir_set.end ();
    }

    /**
     * Return a reference to the Singleton.
     */
    static xml_access_helper & helper()
    {
      return s_exemplar;
    }
    /**
     * The public ID for this document type. This is an indirect reference to the location of a DTD
     * 
     * A public ID is an public label for a DTD.  It is used to look up a DTD in catalogues etc to find local copies of "standard" DTDs.  It is generally given with the real (definitive?) location of the DTD.
     */
    const std::string & public_id() throw ()
    {
      return m_public_id;
    }

    /**
     * The system ID for this document type.
     * 
     * This is a URI for the location of the DTD.  It is a direct URl reference to a DTD 
     * file.  This will return a path relative to the installation's shared data directory.
     */
    const std::string & system_id() throw ()
    {
      return m_system_id;
    }


  private:
    /**
     * Hidden default ctor, use helper() function to get a helper object.
     */
    xml_access_helper()
    : m_auto_fix (false)
    , m_dir_set ()
    , m_public_id (default_public_id ())
    , m_system_id (default_system_id ())
    {
      register_options ();
    }


  public:
    ~xml_access_helper() throw ()
    {}


  private:
    /**
     * Add options to the configuration helper.
     */
    void register_options();

    /**
     * no copy
     */
    xml_access_helper(const xml_access_helper & source);

    /**
     * no copy
     */
    xml_access_helper & operator=(const xml_access_helper & source);

};

} // namespace data_access
#endif
