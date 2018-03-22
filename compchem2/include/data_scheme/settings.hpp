#ifndef COMPCHEM_DATA_SCHEME_SETTINGS_HPP
#define COMPCHEM_DATA_SCHEME_SETTINGS_HPP

// ------------------------------------------------------------
// class: settings
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/basic_datum.hpp"
#include <memory>
#include <string>

namespace data_scheme { class datum; } 
namespace data_scheme { class info; } 
namespace data_scheme { class mechanism; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * Note that this can contain a datum or an info object but not both.
 */
class settings : public basic_datum {
  private:
    /**
     * The datum object of this setting.
     */
    std::auto_ptr<datum> m_datum;

    /**
     * The space separated list of filter IDs that can be used with this setting.
     */
    std::string m_filters;

    /**
     * The info object of these settings
     */
    std::auto_ptr<info> m_info;

    /**
     * The objects unique ID
     */
    std::string m_label;

    /**
     * The space separated list of supported outcomes.
     */
    std::string m_outcomes;

    /**
     * The target mechanism for these settings. (parent)
     */
    mechanism & m_target;


  public:
    /**
     * add_child tests if this object can accept the tagname using the
     * meta_datum object. Only then is the virt_add_child method called. 
     * 
     * default action is to throw an error!
     * 
     * \return /= NULL
     */
    virtual basic_datum & add_child(const std::string & a_name, std::string & a_label);
    /**
     * Add an info object.
     * 
     * \pre not has_data
     * \return /= NULL
     */
    datum & add_datum(std::string a_label);
    /**
     * Add an info object.
     * 
     * \pre not has_information
     * \return /= NULL
     */
    info & add_info(std::string a_label);
    /**
     * Add a filter label to list of what extracted data into database.
     */
    void add_filter_label(const std::string & a_filter);
    /**
     * Add a filter label to list of what extracted data into database.
     */
    void add_outcome_label(const std::string & a_filter);
    /**
     * Get a reference to the information object.
     * 
     * \pre has_information
     */
     datum & data()
    {
      COMPCHEM_REQUIRE(has_data (), "Attempt to access non-existent datum object.");
      return *(m_datum.get ());
    }

    /**
     * Get a reference to the information object.
     * 
     * \pre has_information
     */
    const datum & data() const
    {
      COMPCHEM_REQUIRE(has_data (), "Attempt to access non-existent datum object.");
      return *(m_datum.get ());
    }

    /**
     * The list of filters on this mechanism action.  If this is a standard mech-memo then this will give the list of possible filters.  For a result mech-memo this will list the used filters.
     */
    const std::string& filter_list() const throw ()
    {
      return m_filters;
    }

    /**
     * Add attributes to the XML element
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

    /**
     * Check whether this has a datum sub-element.
     */
    bool has_data() const throw ()
    {
      return NULL != m_datum.get ();
    }

    /**
     * Does this formula have an information object?
     */
    bool has_information() const throw ()
    {
      return NULL != m_info.get ();
    }

    /**
     * Get a reference to the information object.
     * 
     * \pre has_information
     */
     info & information()
    {
      COMPCHEM_REQUIRE(has_information (), "Attempt to access non-existent information object.");
      return *(m_info.get ());
    }

    /**
     * Get a reference to the information object.
     * 
     * \pre has_information
     */
    const info & information() const
    {
      COMPCHEM_REQUIRE(has_information (), "Attempt to access non-existent information object.");
      return *(m_info.get ());
    }

    const std::string& label() const throw ()
    {
      return m_label;
    }

    /**
     * Get the list of outcomes
     */
    const std::string& outcomes() const
    {
      return m_outcomes;
    }

    /**
     * Return a pointer to the parent object, may be NULL.
     */
    const basic_datum & parent() const throw ();

    /**
     * Construct a new setting object with a_parent and a_label.
     * Swallows the content of a_label
     * 
     * \post a_label.empty
     */
    settings(mechanism & a_parent, std::string & a_label);


  private:
    /**
     * no copy 
     */
    settings(const settings & source);


  public:
    ~settings() throw ();


  private:
    /**
     * no assign
     */
    settings & operator=(const settings & source);


  public:
    /**
     * The key for this class during serialisation.
     */
    std::string serial_name() const;

    /**
     * Write an attribute.  Can assume that a_name and a_value are valid as
     * per the definition() object.
     */
    void set_attribute(const std::string & a_name, std::string a_value);


  protected:
    /**
     * Called when an object of a derived class will be removed.
     * 
     * throw exception if a_datum is not a child of derived class.
     */
    virtual void virt_remove_child(basic_datum * a_datum);


  public:
    void visit(visitor & a_visitor) const;

};

} // namespace data_scheme
#endif
