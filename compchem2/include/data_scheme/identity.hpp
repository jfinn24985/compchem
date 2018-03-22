#ifndef COMPCHEM_DATA_SCHEME_IDENTITY_HPP
#define COMPCHEM_DATA_SCHEME_IDENTITY_HPP

// ------------------------------------------------------------
// class: identity
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/basic_datum.hpp"
#include <string>

namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * A compound is universally unique.  Within a program it is given an arbitrary ID.
 * However that are standards for uniquely identifying a compound across programs, eg
 * iupac-name, CAS number etc.  The content of this element contains this information
 * for use in matching compounds across databases.
 */
class identity : public basic_datum {
  private:
    /**
     * The parent of this identity (Compound or Formula)
     */
    basic_datum & m_parent;

    /**
     * The type of identity information.  This is from a standard catalog of identity types
     */
    std::string m_type;


  public:
    /**
     * Add attributes to the XML element
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

    /**
     * Construct a new identity object with a_parent and a_type. Swallows 
     * content of a_type
     * 
     * \post a_type.empty
     */
    identity(basic_datum & a_parent, std::string & a_type)
    : basic_datum ()
    , m_parent (a_parent)
    , m_type ()
    {
      m_type.swap (a_type);
    }


  private:
    /**
     * no copy
     */
    identity(const identity & source);


  public:
    ~identity() throw ();

    /**
     * Get the identity type
     */
    const std::string & identity_type() const
    {
      return m_type;
    }


  private:
    /**
     * no assign
     */
    identity & operator=(const identity & source);


  public:
    /**
     * Return a pointer to the parent object, may be NULL.
     */
    const basic_datum & parent() const throw ()
    {
      return m_parent;
    }

    /**
     * The key for this class during serialisation.
     */
    std::string serial_name() const;

    /**
     * Throw an error as program has no attributes.
     */
    virtual void set_attribute(const std::string & a_name, std::string a_value);

    void visit(visitor & a_visitor) const;

};

} // namespace data_scheme
#endif
