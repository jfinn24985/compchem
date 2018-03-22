#ifndef COMPCHEM_DATA_SCHEME_EXPERIMENT_HPP
#define COMPCHEM_DATA_SCHEME_EXPERIMENT_HPP

// ------------------------------------------------------------
// class: experiment
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/basic_datum.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>


namespace data_scheme { class program; } 
namespace utility { class program_options; } 
namespace data_scheme { class info; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class project; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * The outcome of a set of work done by the chemist/worker. Also the description of
 * what was done.
 */
class experiment : public basic_datum {
  public:
    typedef boost::ptr_vector< info >::const_iterator const_info_iterator;

    typedef boost::ptr_vector< info >::iterator info_iterator;

    typedef boost::ptr_vector< memo >::const_iterator const_memo_iterator;

    typedef boost::ptr_vector< memo >::iterator memo_iterator;


  private:
    /**
     * The starting date of the experiment
     */
    std::string m_date_start;

    /**
     * The finish date for the expt.
     */
    std::string m_date_end;

    /**
     * The information objects for this experiment.
     */
    boost::ptr_vector<info> m_info;

    /**
     * The priority of this experiment. Higher numbers have higher priority, default is 0
     */
    unsigned int m_priority;

    /**
     * A unique identifier. The equivalent of the page number/experiment number in a lab
     * notebook.
     */
    std::string m_reference;

    /**
     * The parts of this experiment.
     */
    boost::ptr_vector<memo> m_results;

    /**
     * The project this experiment was performed in. (parent)
     */
    project & m_section;

    /**
     * Space separated list of worker IDs
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
     * add_info child. 
     * 
     * default action is to throw an error!
     * 
     * \return /= NULL
     */
    info & add_info(std::string a_label);
    /**
     * add_child tests if this object can accept the tagname using the
     * meta_datum object. Only then is the virt_add_child method called. 
     * 
     * default action is to throw an error!
     * 
     * \pre find_memo (a_label) = end_memo
     * \return /= NULL
     */
    memo & add_memo(std::string a_label);
    /**
     * add_child tests if this object can accept the tagname using the
     * meta_datum object. Only then is the virt_add_child method called. 
     * 
     * default action is to throw an error!
     * 
     * \return /= NULL
     */
    memo & add_memo_src(const std::string& a_uri, std::string a_label);

    void add_worker_label(const std::string & a_set);

    info_iterator begin_info()
    {
      return m_info.begin ();
    }

    const_info_iterator begin_info() const
    {
      return m_info.begin ();
    }

    memo_iterator begin_memo()
    {
      return m_results.begin ();
    }

    const_memo_iterator begin_memo() const
    {
      return m_results.begin ();
    }

    /**
     * The starting date of the experiment
     */
    boost::gregorian::date date_start() const;
    /**
     * The starting date of the experiment.  If a_date_str is empty then today's date is used.
     */
    void date_start(const boost::gregorian::date& a_date);
    /**
     * The finish date for the expt.
     */
    boost::gregorian::date date_end() const;
    /**
     * The finish date for the expt. If a_date_str is empty, then value of date_start() is used.
     * 
     * \pre a_date >= date_start ()
     */
    void date_end(const boost::gregorian::date& a_date);
    bool empty_info() const throw ()
    {
      return m_info.empty ();
    }

    bool empty_memo() const throw ()
    {
      return m_results.empty ();
    }

    info_iterator end_info()
    {
      return m_info.end ();
    }

    const_info_iterator end_info() const
    {
      return m_info.end ();
    }

    memo_iterator end_memo()
    {
      return m_results.end ();
    }

    const_memo_iterator end_memo() const
    {
      return m_results.end ();
    }

    /**
     * Construct a new experiment object as a child of a_parent. Swallow
     * a_reference
     * 
     * \post a_reference.empty
     */
    experiment(project & a_parent, std::string & a_reference);


  private:
    /**
     * no copy
     */
    experiment(const experiment & source);


  public:
    ~experiment() throw ();

    /**
     * memo [data_scheme]
     */
    memo_iterator find_memo(const std::string& a_label);

    /**
     * memo [data_scheme]
     */
    const_memo_iterator find_memo(const std::string& a_label) const;

    /**
     * Add attributes to the XML element
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

    bool has_priority() const throw ()
    {
      return m_priority != 0;
    }

    /**
     * Fill in the default information for a newly created
     * object.
     */
    void initialise();


  private:
    /**
     * no assign
     */
    experiment & operator=(const experiment & source);


  public:
    /**
     * Return a pointer to the parent object, may be NULL.
     */
    const basic_datum & parent() const throw ();

    /**
     * Indicate the relative importance of the result set.  If you have several experiments
     * with similar result sets this attribute allows you to identify which sets should be
     * used for generating results.  It is currently a boolean value to indicate the experiment
     * data set to use.
     */
    unsigned int priority() const throw ()
    {
      return m_priority;
    }

    /**
     * Indicate the relative importance of the result set.  If you have several experiments
     * with similar result sets this attribute allows you to identify which sets should be
     * used for generating results.  It is currently a boolean value to indicate the experiment
     * data set to use.
     */
    void priority(unsigned int a_val) throw ()
    {
      m_priority = a_val;
    }

    /**
     * A unique identifier. The equivalent of the page number/experiment number in a lab
     * notebook.
     */
    const std::string& reference() const throw ()
    {
      return m_reference;
    }

    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(memo & a_datum);
    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(info & a_datum);

  private:
    /**
     * Remove a memo object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(memo & a_datum);

    /**
     * Remove a info object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(info & a_datum);


  public:
    /**
     * The parent project
     */
    const project & section() const throw ()
    {
      return m_section;
    }

    /**
     * The XML element definition for this datum.
     */
    std::string serial_name() const;

    /**
     * Write an attribute. Can assume that a_name and a_value are valid as per the
     * definition() object.
     */
    void set_attribute(const std::string & a_name, std::string a_value);

    unsigned int size_info() const throw ()
    {
      return m_info.size ();
    }

    unsigned int size_memo() const throw ()
    {
      return m_results.size ();
    }

    void unset_priority() throw()
    {
      m_priority = 0;
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
     * The list of workers on this experiment.
     */
    const std::string& worker_list() const throw ()
    {
      return m_workers;
    }

};
#include "data_scheme/experiment.hpp"
} // namespace data_scheme
#endif
