#ifndef COMPCHEM_DATA_SCHEME_WORKER_HPP
#define COMPCHEM_DATA_SCHEME_WORKER_HPP

// ------------------------------------------------------------
// class: worker
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

namespace data_scheme { class info; } 
namespace data_scheme { class program; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 *  A person doing the work
 * 
 * This allows you to assign people to programs and projects. 
 */
class worker : public basic_datum {
  public:
    typedef boost::ptr_vector< info >::iterator info_iterator;

    typedef boost::ptr_vector< info >::const_iterator const_info_iterator;


  private:
    /**
     * The set of info objects.
     */
    boost::ptr_vector<info> m_information;

    /**
     * Data to enable cross-referencing an experimenter with an experiment 
     */
    std::string m_label;

    /**
     * The site (parent) this worker works at.
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
     * add_child tests if this object can accept the tagname using the
     * meta_datum object. Only then is the virt_add_child method called. 
     * 
     * default action is to throw an error!
     * 
     * \return /= NULL
     */
    info & add_info(std::string a_label);
    info_iterator begin_info()
    {
      return m_information.begin ();
    }

    const_info_iterator begin_info() const
    {
      return m_information.begin ();
    }

    bool empty_info() const throw ()
    {
      return m_information.empty ();
    }

    info_iterator end_info()
    {
      return m_information.end ();
    }

    const_info_iterator end_info() const
    {
      return m_information.end ();
    }

    /**
     * Add attributes to the XML element
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

    const std::string& label() const throw ()
    {
      return m_label;
    }

    /**
     * Return a pointer to the parent object, may be NULL.
     */
    const basic_datum & parent() const throw ();


  private:
    /**
     * no assign
     */
    worker & operator=(const worker & source);


  public:
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
     * The XML element definition for this datum.
     */
    std::string serial_name() const;


  private:
    /**
     * Write an attribute. Can assume that a_name and a_value are valid as per the
     * definition() object.
     */
    void set_attribute(const std::string & a_name, std::string a_value);


  public:
    unsigned int size_info() const throw ()
    {
      return m_information.size ();
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

    /**
     * Construct a new worker object with a_parent and a_label.
     * Swallow the content of a_label.
     * 
     * \post a_label.empty
     */
    worker(program & a_parent, std::string & a_label);


  private:
    /**
     * no copy
     */
    worker(const worker & source);


  public:
    ~worker() throw ();

};

} // namespace data_scheme
#endif
