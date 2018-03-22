#ifndef COMPCHEM_MANAGER_HPP
#define COMPCHEM_MANAGER_HPP

// ------------------------------------------------------------
// class: manager
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/function.hpp>

#include <memory>
#include <string>
#include <map>

namespace data_access { template<class T_xml_string> class basic_names; } 
namespace data_access { class persistence; } 
namespace data_scheme { class basic_datum; } 

namespace data_access {

/**
 * This classes provides management functions for the data access.
 * Back-ends register handlers for the URI schemes they support. 
 * Front-ends get the appropriate based on the data-base URI.
 */
class manager
{
  public:
    typedef boost::function2< std::auto_ptr< persistence >, data_scheme::basic_datum&, const std::string& > create_persistence_function;


  private:
    /**
     * The map of creation functions.
     */
    std::map<std::string, create_persistence_function> m_creator;


  public:
    /**
     * Create a persistence object that matches the uri.
     * 
     * \pre is_supported (a_uri_scheme)
     */
    std::auto_ptr< persistence > connect(data_scheme::basic_datum & a_top, const std::string & a_uri);

    /**
     * Does this manager have support for the given URI? 
     * 
     * NOTE: This really only checks that the URI scheme/protocol has 
     * a matching persistence creation method.  Whether the rest of the
     * URI is valid requires a connection to be attempted.
     */
    bool is_supported(const std::string& a_uri) const;

  private:
    /**
     * Singleton constructor
     */
    manager() throw ();


  public:
    ~manager() throw ();


  private:
    /**
     * no copy
     */
    manager(const manager & source);

    /**
     * no assign
     */
    manager & operator=(const manager & source);


  public:
    /**
     * Register a_creation_function for a_uri_type with the manager
     * 
     * \pre not is_supported (a_uri_type)
     * \post is_supported (a_uri_type)
     */
    void register_creator(const std::string& a_uri_type, const create_persistence_function a_creation_function)
    {
      COMPCHEM_REQUIRE (not is_supported (a_uri_type), "Cannot add support for a single URI scheme more than once.");
      m_creator [a_uri_type] = a_creation_function;
      COMPCHEM_ENSURE (is_supported (a_uri_type), ("Failed to add support for URI scheme: " + a_uri_type));
    }

    /**
     * Get the manager Singleton object.
     */
    static manager & server() throw ();

    /**
     * The number of supported URI schemes.
     */
    unsigned int size() const
    {
      return m_creator.size ();
    }


};

} // namespace data_access
#endif
