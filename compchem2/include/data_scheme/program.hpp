#ifndef COMPCHEM_DATA_SCHEME_PROGRAM_HPP
#define COMPCHEM_DATA_SCHEME_PROGRAM_HPP

// ------------------------------------------------------------
// class: program
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include "data_scheme/basic_datum.hpp"
#include <memory>
#include <string>

namespace data_scheme { class data_scheme_helper; } 
namespace data_scheme { class compound; } 
namespace data_scheme { class mechanism; } 
namespace data_scheme { class project; } 
namespace data_scheme { class worker; } 
namespace data_access { class persistence; } 
namespace data_scheme { class info; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * A program  of research is a broad category of interest.  It is intended that the
 * contents of a possible paper do not cross program boundaries, although several
 * papers may be contained in a program. 
 */
class program : public basic_datum {
  public:
    /**
     * Iterator type to access the set of compounds
     */
    typedef boost::ptr_vector< compound >::const_iterator const_compound_iterator;

    /**
     * Iterator type to access the set of compounds
     */
    typedef boost::ptr_vector< compound >::iterator compound_iterator;

    /**
     * Iterator type to access the equipment list
     */
    typedef boost::ptr_vector< mechanism >::const_iterator const_mechanism_iterator;
    

    /**
     * Iterator type to access the equipment list
     */
    typedef boost::ptr_vector< mechanism >::iterator mechanism_iterator;
    

    /**
     * Iterator to access list of projects.
     */
    typedef boost::ptr_vector< project >::const_iterator const_project_iterator;
    

    /**
     * Iterator to access list of projects.
     */
    typedef boost::ptr_vector< project >::iterator project_iterator;
    

    /**
     * Iterator to access list of workers
     */
    typedef boost::ptr_vector< worker >::const_iterator const_worker_iterator;
    

    /**
     * Iterator to access list of workers
     */
    typedef boost::ptr_vector< worker >::iterator worker_iterator;
    


  private:
    /**
     * The list of compounds for this site.
     */
    boost::ptr_vector<compound> m_catalog;

    /**
     * The XML document this program is related to.  It is nul when no document is associated.
     */
    std::auto_ptr<data_access::persistence> m_document;

    /**
     * The result mechanisms for this site.
     */
    boost::ptr_vector<mechanism> m_equipment;

    /**
     * Information about this research program
     */
    std::auto_ptr<info> m_info;

    /**
     * The projects in this site.
     */
    boost::ptr_vector<project> m_projects;

    /**
     * The staff in this program.
     */
    boost::ptr_vector<worker> m_staff;


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
     * Create and add a worker object.
     * 
     * \return /= nul
     */
    compound & add_compound(std::string a_label);

    /**
     * Add an info object.
     * 
     * \pre not has_information
     * \return /= NULL
     */
    info & add_info(std::string a_label);
    /**
     * Create and add a worker object.
     * 
     * \return /= nul
     */
    mechanism & add_mechanism(std::string a_label);

    /**
     * Create and add a worker object.
     * 
     * \return /= nul
     */
    project & add_project(std::string a_label);

    /**
     * Create and add a worker object.
     * 
     * \return /= nul
     */
    project & add_project_src(const std::string& a_uri, std::string a_label);

    /**
     * Create and add a worker object.
     * 
     * \return /= nul
     */
    worker & add_worker(std::string a_label);

    /**
     * view   [data_scheme]
     */
    compound_iterator begin_compound()
    {
      return m_catalog.begin ();
    }

    /**
     * view   [data_scheme]
     */
    const_compound_iterator begin_compound() const
    {
      return m_catalog.begin ();
    }

    /**
     * mechanism   [data_scheme]
     */
    mechanism_iterator begin_mechanism()
    {
      return m_equipment.begin ();
    }

    /**
     * mechanism   [data_scheme]
     */
    const_mechanism_iterator begin_mechanism() const
    {
      return m_equipment.begin ();
    }

    /**
     * project   [data_scheme]
     */
    project_iterator begin_project()
    {
      return m_projects.begin ();
    }

    /**
     * project   [data_scheme]
     */
    const_project_iterator begin_project() const
    {
      return m_projects.begin ();
    }

    /**
     * worker   [data_scheme]
     */
    worker_iterator begin_worker()
    {
      return m_staff.begin ();
    }

    /**
     * worker   [data_scheme]
     */
    const_worker_iterator begin_worker() const
    {
      return m_staff.begin ();
    }

    /**
     * Second part of constructor connects the program object to the serialisation source.
     */
    void connect(const std::string& a_uri);

    bool empty_compound() const
    {
      return m_catalog.empty ();
    }

    bool empty_mechanism() const
    {
      return m_equipment.empty ();
    }

    bool empty_project() const
    {
      return m_projects.empty ();
    }

    bool empty_worker() const
    {
      return m_staff.empty ();
    }

    /**
     * view   [data_scheme]
     */
    compound_iterator end_compound()
    {
      return m_catalog.end ();
    }

    /**
     * view   [data_scheme]
     */
    const_compound_iterator end_compound() const
    {
      return m_catalog.end ();
    }

    /**
     * mechanism   [data_scheme]
     */
    mechanism_iterator end_mechanism()
    {
      return m_equipment.end ();
    }

    /**
     * mechanism   [data_scheme]
     */
    const_mechanism_iterator end_mechanism() const
    {
      return m_equipment.end ();
    }

    /**
     * project   [data_scheme]
     */
    project_iterator end_project()
    {
      return m_projects.end ();
    }

    /**
     * project   [data_scheme]
     */
    const_project_iterator end_project() const
    {
      return m_projects.end ();
    }

    /**
     * worker   [data_scheme]
     */
    worker_iterator end_worker()
    {
      return m_staff.end ();
    }

    /**
     * worker   [data_scheme]
     */
    const_worker_iterator end_worker() const
    {
      return m_staff.end ();
    }

    /**
     * compound   [data_scheme]
     */
    compound_iterator find_compound(const std::string & a_label);

    /**
     * compound   [data_scheme]
     */
    const_compound_iterator find_compound(const std::string & a_label) const;

    /**
     * mechanism [data_scheme]
     */
    mechanism_iterator find_mechanism(const std::string & a_label);

    /**
     * mechanism [data_scheme]
     */
    const_mechanism_iterator find_mechanism(const std::string & a_label) const;

    /**
     * Find an existing project element. This either returns a null element or a 'project' element.  This requires that a 'project-src' element will be followed.
     */
    project_iterator find_project(const std::string & a_label);

    /**
     * Find an existing project element. This either returns a null element or a 'project' element.  This requires that a 'project-src' element will be followed.
     */
    const_project_iterator find_project(const std::string & a_label) const;

    /**
     * worker   [data_scheme]
     */
    worker_iterator find_worker(const std::string & a_label);

    /**
     * worker   [data_scheme]
     */
    const_worker_iterator find_worker(const std::string & a_label) const;

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


  private:
    /**
     * no assign
     */
    program & operator=(const program & source);


  public:
    /**
     * Throw an error as program objects do not have parents.
     * 
     * Note: Returns a pointer to the parent object to compile 
     * without warnings.
     */
    const basic_datum & parent() const
    {
      const bool program_has_no_parent (false);
      COMPCHEM_ALWAYS (program_has_no_parent, serial_name () + " objects do not parents");
      return *this;
    }

    /**
     * Default program constructor has no arguments?
     */
    explicit program();


  private:
    /**
     * no copy
     */
    program(const program & source);


  public:
    ~program() throw ();

    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(compound & a_datum);
    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(mechanism & a_datum);
    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(project & a_datum);
    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(worker & a_datum);

  private:
    /**
     * Remove a formula object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(compound & a_datum);

    /**
     * Remove a formula object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(mechanism & a_datum);

    /**
     * Remove a formula object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(project & a_datum);

    /**
     * Remove a formula object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(worker & a_datum);


  public:
    /**
     * The key for this class during serialisation.
     */
    std::string serial_name() const;

    /**
     * Throw an error as program has no attributes.
     */
    virtual void set_attribute(const std::string & a_name, std::string a_value);

    unsigned int size_compound() const
    {
      return m_catalog.size ();
    }

    unsigned int size_mechanism() const
    {
      return m_equipment.size ();
    }

    unsigned int size_project() const
    {
      return m_projects.size ();
    }

    unsigned int size_worker() const
    {
      return m_staff.size ();
    }

    /**
     * Attempt to find the project named by data_scheme_helper.target_project.
     * Return end_project when project is not found (also auto_project)
     */
    project_iterator target_project();

    /**
     * Get the full path to the document/source that contains the current element 
     * (also see basic_datum::uri_path)
     */
    virtual std::string uri_path() const;

    /**
     * Get the scheme used to connect to the document/source that contains 
     * the current element. (also see basic_datum::uri_scheme)
     */
    virtual std::string uri_scheme() const;


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
     * Trigger document to write itself.
     * 
     * \pre is_src
     */
    void write() const;

};

} // namespace data_scheme
#endif
