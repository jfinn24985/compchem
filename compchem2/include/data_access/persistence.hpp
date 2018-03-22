#ifndef COMPCHEM_PERSISTENCE_HPP
#define COMPCHEM_PERSISTENCE_HPP

// ------------------------------------------------------------
// class: persistence
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>

namespace data_scheme { class basic_datum; } 

namespace data_access {

/**
 * The base class for objects that lead to persistence of a class.
 */
class persistence
{
  private:
    /**
     * Reference to the owning object.
     */
    data_scheme::basic_datum & m_top;


  public:
    /**
     * Default ctor
     */
    persistence(data_scheme::basic_datum & a_top) throw ()
    : m_top (a_top)
    {}

    virtual ~persistence() throw ();


  private:
    /**
     * no copy
     */
    persistence(const persistence & source);

    /**
     * no assign
     */
    persistence & operator=(const persistence & source);


  public:
    /**
     * A reference to the top of the managed object tree.
     */
    data_scheme::basic_datum & top() throw ()
    {
      return m_top;
    }

    /**
     * A reference to the top of the managed object tree.
     */
    const data_scheme::basic_datum & top() const throw ()
    {
      return m_top;
    }

    /**
     * Deserialise data from persistent storage
     */
    virtual void read() = 0;

    /**
     * The URI path of the persistent storage. This is the fully qualified path
     * and not a relative path. It is represented as a string.
     */
    virtual std::string uri_path() const = 0;

    /**
     * The URI scheme of the persistent storage. Currently this will be 
     * 'file' for XML file. Future extensions to 'odbc' for databases is planned.
     */
    virtual std::string uri_scheme() const = 0;

    /**
     * The combination of the URI scheme and a (possibly relative) path to the 
     * persistent storage. 
     */
    virtual std::string uri() const = 0;

    /**
     * Serialise data to persistent storage.
     */
    virtual void write() const = 0;


};

} // namespace data_access
#endif
