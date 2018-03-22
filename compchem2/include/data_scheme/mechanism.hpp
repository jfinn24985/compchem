#ifndef COMPCHEM_DATA_SCHEME_MECHANISM_HPP
#define COMPCHEM_DATA_SCHEME_MECHANISM_HPP

// ------------------------------------------------------------
// class: mechanism
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/basic_datum.hpp"
#include <boost/bind.hpp>

#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include <memory>

namespace data_scheme { class filter; } 
namespace data_scheme { class settings; } 
namespace data_scheme { class info; } 
namespace data_scheme { class program; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * This contain information about something that produces results. It can be a computer 
 * program or a piece of lab equipemnt. The content should be information that is
 * common to every time this "mechanism" is referred to. This might be the bibliographic
 * reference for a programme or the manufacturers address and model number. Set up parameters
 * may be included if they are common to every usage (such as detector type). Usage specific 
 * information is contained in elements below the "experiment" element.
 * 
 * NOTE The contents of this element can also refer to a set of published results. 
 * \final
 */
class mechanism : public basic_datum {
  public:
    typedef boost::ptr_vector< filter >::iterator filter_iterator;

    typedef boost::ptr_vector< filter >::const_iterator const_filter_iterator;

    typedef boost::ptr_vector< settings >::iterator settings_iterator;

    typedef boost::ptr_vector< settings >::const_iterator const_settings_iterator;


  private:
    /**
     * A group of settings to perform some action.
     */
    boost::ptr_vector<settings> m_command_set;

    /**
     * The cross-reference label for this mechanism
     */
    std::string m_label;

    /**
     * The mechanism information
     */
    std::auto_ptr<info> m_info;

    /**
     * The set of filters applicable to this mechanism.
     */
    boost::ptr_vector<filter> m_filter_set;

    /**
     * The site this mechanism is located at. (parent)
     */
    program & m_site;


  public:
    /**
     * add_child tests if this object can accept the tagname using the
     * meta_datum object. Only then is the virt_add_child method called. 
     * 
     * default action is to throw an error!
     * 
     * \return /= NULL
     */
    basic_datum & add_child(const std::string & a_name, std::string & a_label);
    /**
     * Add an info object.
     * 
     * \pre not has_information
     * \return /= NULL
     */
    info & add_info(std::string a_label);
    /**
     * Create and add a filter.
     * 
     * \pre find_filter(a_label) = end_label
     * \return /= NULL
     */
    filter & add_filter(std::string a_label);
    /**
     * Create and add a settings child object.
     * 
     * \pre find_settings(a_label) = end_settings
     * \return /= NULL
     */
    settings & add_setting(std::string a_label);
    filter_iterator begin_filter()
    {
      return m_filter_set.begin ();
    }

    const_filter_iterator begin_filter() const
    {
      return m_filter_set.begin ();
    }

    settings_iterator begin_settings()
    {
      return m_command_set.begin ();
    }

    const_settings_iterator begin_settings() const
    {
      return m_command_set.begin ();
    }

    bool empty_filter() const throw ()
    {
      return m_filter_set.empty ();
    }

    bool empty_settings() const throw ()
    {
      return m_command_set.empty ();
    }

    filter_iterator end_filter()
    {
      return m_filter_set.end ();
    }

    const_filter_iterator end_filter() const
    {
      return m_filter_set.end ();
    }

    settings_iterator end_settings()
    {
      return m_command_set.end ();
    }

    const_settings_iterator end_settings() const
    {
      return m_command_set.end ();
    }

    /**
     * Find filter with matching label
     */
    filter_iterator find_filter(const std::string & a_label);

    /**
     * Find filter with matching label
     */
    const_filter_iterator find_filter(const std::string & a_label) const;

    /**
     * Add attributes to the XML element
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

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

    const std::string & label() const throw ()
    {
      return m_label;
    }

    /**
     * Construct a new mechanism object with a_parent and a_label. Swallow
     * content of a_label
     * 
     * \post a_label.empty
     */
    mechanism(program & a_parent, std::string & a_label);

  private:
    /**
     * no copy
     */
    mechanism(const mechanism & source);


  public:
    ~mechanism() throw ();


  private:
    /**
     * no assign
     */
    mechanism & operator=(const mechanism & source);


  public:
    /**
     * Return a pointer to the parent object, may be NULL.
     */
    const basic_datum & parent() const throw ();

    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(filter & a_datum);
    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(settings & a_datum);

  private:
    /**
     * Remove a filter object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(filter & a_datum);

    /**
     * Remove a info object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(settings & a_datum);


  public:
    /**
     * The key for this class during serialisation.
     */
    std::string serial_name() const;

    /**
     * Throw an error as program has no attributes.
     */
    virtual void set_attribute(const std::string & a_name, std::string a_value);

    unsigned int size_filter() const throw ()
    {
      return m_filter_set.size ();
    }

    unsigned int size_settings() const throw ()
    {
      return m_command_set.size ();
    }


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
