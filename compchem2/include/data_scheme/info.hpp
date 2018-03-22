#ifndef COMPCHEM_DATA_SCHEME_INFO_HPP
#define COMPCHEM_DATA_SCHEME_INFO_HPP

// ------------------------------------------------------------
// class: info
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/basic_datum.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include "data_scheme/experiment.hpp"

namespace data_scheme { class visitor; } 

namespace data_scheme {

class info : public basic_datum {
  public:
    typedef boost::ptr_vector< info >::iterator info_iterator;

    typedef boost::ptr_vector< info >::const_iterator const_info_iterator;


  private:
    /**
     * The content of this information
     */
    boost::ptr_vector<info> m_content;

    std::string m_name;

    /**
     * The parent of this object
     */
    basic_datum & m_parent;


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
     * add_child tests if this object can accept the tagname using the
     * meta_datum object. Only then is the virt_add_child method called. 
     * 
     * \return /= NULL
     */
    info & add_info(std::string a_label);
    info_iterator begin_info()
    {
      return m_content.begin ();
    }

    const_info_iterator begin_info() const
    {
      return m_content.begin ();
    }

    bool empty_info() const throw ()
    {
      return m_content.empty ();
    }

    info_iterator end_info()
    {
      return m_content.end ();
    }

    experiment::const_info_iterator end_info() const
    {
      return m_content.end ();
    }

    /**
     * Add attributes to the XML element
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

    /**
     * Construct a new info object with a_parent and a_name. Swallow
     * content of a_name
     * 
     * \post a_name.empty
     */
    info(basic_datum & a_parent, std::string & a_name)
    : basic_datum()
    , m_content ()
    , m_name ()
    , m_parent(a_parent)
    {
      m_name.swap (a_name);
    }


  private:
    /**
     * no copy
     */
    info(const info & source);


  public:
    ~info() throw ()
    {}

    const std::string & name() const throw ()
    {
      return m_name;
    }

    void name(std::string a_str) throw ()
    {
      m_name.swap (a_str);
    }

  private:
    /**
     * no copy
     */
    info & operator=(const info & source);


  public:
    /**
     * Return a pointer to the parent object, may be NULL.
     */
    const basic_datum & parent() const throw ()
    {
      return m_parent;
    }

    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(info & a_datum);

  private:
    /**
     * Remove a info object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(info & a_datum);


  public:
    /**
     * The key for this class during serialisation.
     */
    std::string serial_name() const;

    /**
     * Write an attribute.  Throw an error if arguments are incorrect.
     */
    virtual void set_attribute(const std::string & a_name, std::string a_value);

    unsigned int size_info() const throw ()
    {
      return m_content.size ();
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
