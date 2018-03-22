// ------------------------------------------------------------
// class: manager
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_access/manager.hpp"
#include "data_access/names.hpp"
#include "data_access/persistence.hpp"
#include "data_scheme/basic_datum.hpp"

// Manual Includes
#include "xmlreader/xml2_uri.hpp"
// -
namespace data_access {

/**
 * Create a persistence object that matches the uri.
 * 
 * \pre is_supported (a_uri_scheme)
 */
std::auto_ptr< persistence > manager::connect(data_scheme::basic_datum & a_top, const std::string & a_uri) 
{
  COMPCHEM_REQUIRE(is_supported (a_uri), "Unsupported URI scheme, can not create persistence object.");
  // Assume is a plain filename if URI scheme is empty
  const xmlreader::uri l_uri (a_uri);
  const std::string l_uri_scheme (l_uri.scheme ());
  const std::string l_scheme (l_uri_scheme.empty ()? names::file_scheme (): l_uri_scheme);
  std::auto_ptr< persistence > Result;
  return m_creator[l_scheme] (a_top, a_uri);

}

/**
 * Does this manager have support for the given URI? 
 * 
 * NOTE: This really only checks that the URI scheme/protocol has 
 * a matching persistence creation method.  Whether the rest of the
 * URI is valid requires a connection to be attempted.
 */
bool manager::is_supported(const std::string& a_uri) const 
{
  std::auto_ptr< xmlreader::uri > l_uri (xmlreader::uri::make_uri (a_uri));
  return (NULL != l_uri.get () and
           ((l_uri->scheme ().empty () and 0 != m_creator.count (names::file_scheme ()))
             or 0 != m_creator.count (l_uri->scheme ())));

}

/**
 * Singleton constructor
 */
manager::manager() throw () 
: m_creator ()
{}

manager::~manager() throw () 
{}

/**
 * Get the manager Singleton object.
 */
manager & manager::server() throw ()

{
  static manager s_exemplar;
  return s_exemplar;

}


} // namespace data_access
