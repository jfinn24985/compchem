#ifndef COMPCHEM_DATA_SCHEME_FILTER_HPP
#define COMPCHEM_DATA_SCHEME_FILTER_HPP

// ------------------------------------------------------------
// class: filter
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

namespace data_scheme { class mechanism; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * This element contains information about an  
 * external filter that is available for operating on 
 * files created by the mechanism.
 * 
 * This element caches information about an
 * external filter that is available for
 * operating on files created by the
 * mechanism to import outcomes into the DB.
 */
class filter : public basic_datum {
  private:
    /**
     * The filter label/ID
     */
    std::string m_label;

    /**
     * The location/URL of the filter
     */
    std::string m_href;

    /**
     * space separated list of outcomes.
     */
    std::string m_outcomes;

    /**
     * The mechanism this filter works on.
     */
    mechanism & m_target;

    /**
     * The type of the filter. Currently supported are 'plugin' 
     * and 'regex'. Coming soon are 'shell' and 'python'.
     */
    std::string m_type;


  public:
    void add_outcome_label(const std::string & a_set);

    /**
     * Construct a new filter object with a_parent and a_label. Swallows
     * a_label.
     * 
     * \post a_label
     */
    filter(mechanism & a_parent, std::string & a_label);


  private:
    /**
     * no copy
     */
    filter(const filter & source);


  public:
    ~filter() throw ();

    const std::string & filter_type() const throw ();

    void filter_type(std::string a_type) throw ()
    {
      m_type.swap (a_type);
    }
    

    /**
     * Add attributes to the XML element
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

    bool is_default_type() const throw ()
    {
      return m_type.empty ();
    }

    const std::string & href() const throw ()
    {
      return m_href;
    }

    void href(std::string a_str) throw ()
    {
      m_href.swap (a_str);
    }
    const std::string & label() const throw ()
    {
      return m_label;
    }


  private:
    /**
     * no assign
     */
    filter& operator=(const filter & source);


  public:
    const std::string & outcomes() const throw ()
    {
      return m_outcomes;
    }

    /**
     * Return a pointer to the parent object
     */
    const mechanism & parent() const throw ();
    /**
     * Sets the type to the default (="plugin") type.
     */
    void reset_type()
    {
      m_type.clear ();
    }

    /**
     * The key for this class during serialisation
     */
    std::string serial_name() const;

    /**
     * Write an attribute. Can assume that a_name and a_value are valid as per the
     * definition() object.
     */
    virtual void set_attribute(const std::string & a_name, std::string a_value);

    void visit(visitor & a_visitor) const;

};

} // namespace data_scheme
#endif
