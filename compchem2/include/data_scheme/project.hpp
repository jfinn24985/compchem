#ifndef COMPCHEM_DATA_SCHEME_PROJECT_HPP
#define COMPCHEM_DATA_SCHEME_PROJECT_HPP

// ------------------------------------------------------------
// class: project
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
#include <memory>
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>



namespace data_scheme { class data_scheme_helper; } 
namespace data_scheme { class experiment; } 
namespace data_scheme { class compound_ref; } 
namespace data_access { class persistence; } 
namespace data_scheme { class info; } 
namespace data_scheme { class program; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * Base type for project related types.
 */
class project : public basic_datum {
  public:
    typedef boost::ptr_vector< experiment >::iterator experiment_iterator;

    typedef boost::ptr_vector< experiment >::const_iterator const_experiment_iterator;

    typedef boost::ptr_vector< compound_ref >::iterator compound_ref_iterator;

    typedef boost::ptr_vector< compound_ref >::const_iterator const_compound_ref_iterator;


  private:
    /**
     * The experiments in this project.
     */
    boost::ptr_vector<experiment> m_activities;

    std::auto_ptr<data_access::persistence> m_document;

    /**
     * The set of imported compounds
     */
    boost::ptr_vector<compound_ref> m_imports;

    /**
     * Information describing this project
     */
    std::auto_ptr<info> m_info;

    /**
     * If this is a proj-src, has it been read in. Also
     * true for a project that has been changed into a
     * proj-src type.
     */
    bool m_is_open;

    /**
     * The project ID 
     */
    std::string m_label;

    /**
     * The set of target outcomes, which in turn implies the set of desired filters.
     */
    std::string m_outcomes;

    /**
     * Parent program
     */
    program & m_site;

    /**
     * The workers on this project
     */
    std::string m_workers;


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
     * create and return a compound_ref, with a_label, as a child of
     * this project.
     * 
     * \return /= NULL
     */
    compound_ref & add_compound_ref(std::string a_label);

    /**
     * Add an info object.
     * 
     * \pre not has_information
     * \return /= NULL
     */
    info & add_info(std::string a_label);
    /**
     * add_child tests if this object can accept the tagname using the
     * meta_datum object. Only then is the virt_add_child method called. 
     * 
     * default action is to throw an error!
     * 
     * \return /= NULL
     */
    experiment & add_experiment(std::string a_label);

    void add_outcome_label(const std::string & a_set);

    /**
     * worker   [data_scheme]
     */
    void add_worker_label(const std::string & a_worker);

    experiment_iterator begin_experiment()
    {
      return m_activities.begin ();
    }

    const_experiment_iterator begin_experiment() const
    {
      return m_activities.begin ();
    }

    /**
     * Get the first ref of a compound to import.
     */
    compound_ref_iterator begin_compound_ref()
    {
      return m_imports.begin ();
    }

    /**
     * Get the first ref of a compound to import.
     */
    const_compound_ref_iterator begin_compound_ref() const
    {
      return m_imports.begin ();
    }

    /**
     * TODO Flush data to disk and revert to unopenned state. Freeing
     * any allocated resources.
     * 
     * \pre is_src and is_open
     * (\post not is_open)
     * 
     * TODO: currently does nothing!
     */
    void close();

    /**
     * Second part of constructor for proj-src derived objects. This
     * must be called after the contructor to create the document.
     */
    void connect(const std::string & a_uri);

    bool empty_experiment() const throw ()
    {
      return m_activities.empty ();
    }

    /**
     * Get the first ref of a compound to import.
     */
    bool empty_compound_ref() const throw ()
    {
      return m_imports.empty ();
    }

    experiment_iterator end_experiment()
    {
      return m_activities.end ();
    }

    const_experiment_iterator end_experiment() const
    {
      return m_activities.end ();
    }

    /**
     * Get the first ref of a compound to import.
     */
    compound_ref_iterator end_compound_ref()
    {
      return m_imports.end ();
    }

    /**
     * Get the first ref of a compound to import.
     */
    const_compound_ref_iterator end_compound_ref() const
    {
      return m_imports.end ();
    }

    /**
     * experiment [data_scheme]
     */
    experiment_iterator find_experiment(const std::string & a_label);

    /**
     * experiment [data_scheme]
     */
    const_experiment_iterator find_experiment(const std::string & a_label) const;

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

    /**
     * Ensure that a newly created object has all the required
     * attributes and children by creating them if necessary.
     */
    void initialise() {}

    /**
     * Has the sub-file been openned?
     * 
     * Only meaningful is is_src = true.
     */
    bool is_open() const throw ()
    {
      return m_is_open;
    }

    /**
     * Is this derived from a memo-src element?
     */
    bool is_src() const throw ()
    {
      return NULL != m_document.get ();
    }

    const std::string & label() const throw ()
    {
      return m_label;
    }

    /**
     * Convert the memo into a src type.
     * 
     * \pre not is_src and ((a_base.empty and exists(parent.database_path)) or exists(a_base))
     * \post is_src
     */
    void make_src(const boost::filesystem::path & a_path);


  private:
    /**
     * no assign
     */
    project & operator=(const project & source);


  public:
    /**
     * Set of target outcomes
     */
    const std::string & outcomes() const throw ()
    {
      return m_outcomes;
    }

    /**
     * Reference to the parent object.
     */
    const basic_datum & parent() const throw ();

    /**
     * Constructor for project object (from project elements) with a_program
     * parent and a_label. Swallows content of a_label.
     * 
     * \post a_label.empty
     */
    project(program & a_program, std::string & a_label);


  private:
    /**
     * no copy
     */
    project(const project & source);


  public:
    ~project() throw ();

    /**
     * Open a src type
     * 
     * \pre is_src
     */
    void read();

    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(experiment & a_datum);
    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(compound_ref & a_datum);

  private:
    /**
     * Remove a info object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(experiment & a_datum);

    /**
     * Remove a info object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(compound_ref & a_datum);


  public:
    /**
     * The key for this class during serialisation.
     */
    std::string serial_name() const;

    /**
     * The key for this class during serialisation.
     */
    std::string serial_src_name() const;

    /**
     * Write an attribute. Can assume that a_name and a_value are valid as per the
     * definition() object.
     */
    void set_attribute(const std::string & a_name, std::string a_value);

    /**
     * Get the project's parent site.
     */
    const program & site() const throw ()
    {
      return m_site;
    }

    unsigned int size_experiment() const throw ()
    {
      return m_activities.size ();
    }

    /**
     * Get the first ref of a compound to import.
     */
    unsigned int size_compound_ref() const throw ()
    {
      return m_imports.size ();
    }

    /**
     * Get the full path to the document/source that contains the current element
     */
    virtual std::string uri_path() const;

    /**
     * Get the scheme used to connect to the document/source that contains 
     * the current element.
     */
    virtual std::string uri_scheme() const;

    /**
     * The URI of the document (proj-src nodes)
     * 
     * \pre is_src
     */
    std::string uri() const;


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
     * List of workers involved in this project (people allowed to edit?)
     */
    const std::string & workers() const throw ()
    {
      return m_workers;
    }

    /**
     * Trigger document to write itself.
     * 
     * \pre is_src
     */
    void write() const;

};

} // namespace data_scheme
#endif
