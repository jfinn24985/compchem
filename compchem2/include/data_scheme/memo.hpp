#ifndef COMPCHEM_DATA_SCHEME_MEMO_HPP
#define COMPCHEM_DATA_SCHEME_MEMO_HPP

// ------------------------------------------------------------
// class: memo
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
#include <memory>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>



namespace data_scheme { class data_scheme_helper; } 
namespace data_scheme { class project; } 
namespace data_scheme { class compound; } 
namespace data_scheme { class datum; } 
namespace data_scheme { class info; } 
namespace data_access { class persistence; } 
namespace data_scheme { class experiment; } 
namespace data_scheme { class formula; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * This contains important settings for the operation that was performed to generate the 
 * results. It should provide details specific to this experiment and include all critical
 * input parameters.
 *                          
 * For computational chemistry this would mean details of the method, basis-set and
 * calculation type. 
 * 
 * NOTE: in Version 1 of DTD this information was contained in the "calc-method", "comp-program"
 * and "lab-app" elements. These elements have been replaced by this single element.
 * 
 * There might be no content in this element if all required information can be derived
 * from the "mechanism" element.
 */
class memo : public basic_datum {
  public:
    typedef boost::ptr_vector< datum >::iterator datum_iterator;

    typedef boost::ptr_vector< datum >::const_iterator const_datum_iterator;

    typedef boost::ptr_vector< info >::iterator info_iterator;

    typedef boost::ptr_vector< info >::const_iterator const_info_iterator;


  private:
    /**
     * The compound this memo hold data about.
     */
    std::string m_compound;

    /**
     * The document associated for memo-src objects.
     */
    std::auto_ptr<data_access::persistence> m_document;

    /**
     * Space separated list of applied filters.
     */
    std::string m_filters;

    /**
     * The ids of any compounds and the formulae this
     * set of results is attributed to.
     */
    std::string m_formula;

    /**
     * A URL pointing to an external file containing the raw data.
     */
    std::string m_href;

    /**
     * The textual content of the memo (if any)
     */
    boost::ptr_vector<info> m_info;

    /**
     * Indicate if a memo-src document has been "read" or
     * that a memo object has been converted to a memo-src
     * object.
     */
    bool m_is_open;

    /**
     * A name that identifies this memento.
     * 
     * This node has two related states.  When used as a child of 
     * 'mechanism' the content defines a standard method or set of
     * operating conditions.  The @name is then a cross-reference 
     * target.  When used as a child of 'experiment' the content 
     * defines the actions that where actually performed. Then 
     * @name is a cross-reference reference to a 'mech-memento' 
     * child of the @mechanism referenced node.  In this
     *      [eg /../mechanism(@label=@mechanism)/mech-memento(@name=@name)]
     * situation the contents of the current node might only 
     * contain information that
     * relates to how a standard method was altered (if any) in this experiment.
     */
    std::string m_label;

    /**
     * The mechanism that was used to produce the contents of this memo.
     */
    std::string m_mechanism;

    /**
     * The experiment this part is from. (parent)
     */
    experiment & m_page;

    /**
     * The datum content of this memo (if any)
     */
    boost::ptr_vector<datum> m_results;


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
     * Create and add a datum object.
     * 
     * \return /= NULL
     */
    virtual datum & add_datum(std::string a_label);
    /**
     * Add a filter label to list of what extracted data into database.
     */
    void add_filter_label(const std::string & a_filter);

    /**
     * Create and add an info object.
     * 
     * \return /= NULL
     */
    virtual info & add_info(std::string a_label);

    /**
     * Get iterator to beginning of datum object list
     */
    datum_iterator begin_datum()
    {
      return m_results.begin ();
    }

    /**
     * Get iterator to beginning of datum object list
     */
    const_datum_iterator begin_datum() const
    {
      return m_results.begin ();
    }

    /**
     * Get iterator to beginning of info object list
     */
    info_iterator begin_info()
    {
      return m_info.begin ();
    }

    /**
     * Get iterator to beginning of info object list
     */
    const_info_iterator begin_info() const
    {
      return m_info.begin ();
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
     * Get the ID of the compound this memo references
     */
    const std::string & compound_label() const
    {
      return m_compound;
    }

    /**
     * Set the compound and formula attributes. This is the only
     * way to change the compound/formula reference of a memo
     * object.
     */
    void compound_formula(const formula & a_form);

    /**
     * Second part of constructor for memo-src objects. This sets the document
     * from a_uri and optionally reads the referenced document depending on
     * data_access::helper().auto_open_memo ()
     * 
     * \post a_uri.empty
     */
    void connect(const std::string & a_uri);

    /**
     * Are there no datum objects?
     */
    bool empty_datum() const throw ()
    {
      return m_results.empty ();
    }

    /**
     * Are there no info objects?
     */
    bool empty_info() const throw ()
    {
      return m_info.empty ();
    }

    /**
     * Get one-past-end iterator of datum object list
     */
    datum_iterator end_datum()
    {
      return m_results.end ();
    }

    /**
     * Get one-past-end iterator of datum object list
     */
    const_datum_iterator end_datum() const
    {
      return m_results.end ();
    }

    /**
     * Get one-past-end iterator of info object list
     */
    info_iterator end_info()
    {
      return m_info.end ();
    }

    /**
     * Get one-past-end iterator of info object list
     */
    const_info_iterator end_info() const
    {
      return m_info.end ();
    }

    /**
     * The list of filters used to produce the contained results.
     */
    const std::string& filters() const throw ()
    {
      return m_filters;
    }

    /**
     * Search for the first datum that has a_outcome. Start search from the
     * optional a_iter (defaults to begin_datum)
     */
    datum_iterator find_datum(const std::string& a_outcome)
    {
      return find_datum (a_outcome, begin_datum ());
    }

    /**
     * Search for the first datum that has a_outcome. Start search from the
     * optional a_iter (defaults to begin_datum)
     */
    datum_iterator find_datum(const std::string& a_outcome, const datum_iterator & a_iter);

    /**
     * Search for the first datum that has a_outcome. Start search from the
     * optional a_iter (defaults to begin_datum)
     */
    const_datum_iterator find_datum(const std::string& a_outcome) const
    {
      return find_datum (a_outcome, begin_datum ());
    }

    /**
     * Search for the first datum that has a_outcome. Start search from the
     * optional a_iter (defaults to begin_datum)
     */
    const_datum_iterator find_datum(const std::string& a_outcome, const const_datum_iterator & a_iter) const;

    /**
     * Get the ID of the formula that this memo references. This formula
     * is a child of the memo's compound reference.
     */
    const std::string & formula_label() const throw ()
    {
      return m_formula;
    }

    /**
     * Get attributes by name. This returns values without checking
     * whether the object is a memo or memo-src object.
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

    /**
     * Get the memo @href attribute.
     * 
     * \return may be empty
     */
    const std::string& href() const throw ()
    {
      return m_href;
    }

    /**
     * Set the href attribute
     * 
     * \pre not is_src or is_open
     */
    void href(std::string a_type) throw ()
    {
      COMPCHEM_REQUIRE(not is_src () or is_open (), "can not set href attribute of closed memo-src object.");
      m_href.swap (a_type);
    }

    /**
     * Has the sub-file been openned?
     * 
     * Only meaningful if is_src = true
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

    /**
     * Get the memo label
     * 
     * \return never empty
     */
    const std::string& label() const throw ()
    {
      return m_label;
    }

    /**
     * Convert the memo into a src type. This requires that the target file
     * does not exist. The action of this method is similar to 'connect'
     * except it sets the reference document as new (and doesn't auto-read!)
     * 
     * \pre not is_src and ((a_base.empty and exists(parent.database_path)) or exists(a_base))
     * \post is_src
     */
    void make_src(const boost::filesystem::path & a_path);

    /**
     * Constructor for memo objects (from memo elements) using a_parent and
     * a_label. Swallows a_label content.
     * 
     * \post a_label.empty
     */
    memo(experiment & a_parent, std::string & a_label);


  private:
    /**
     * no copy
     */
    memo(const memo & source);


  public:
    ~memo() throw ();


  private:
    /**
     * no assign
     */
    memo & operator=(const memo & source);

  public:
    /**
     * Get the parent experiment page
     */
    const experiment & page() const throw ()
    {
      return m_page;
    }

    /**
     * The parent object. The result of this is the same object as page().
     */
    const basic_datum & parent() const throw ();

    /**
     * Open a src type
     * 
     * \pre is_src and not is_open
     */
    void read();

    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(datum & a_datum);
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
    void remove_priv(datum & a_datum);

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
     * The key for this class during serialisation.
     */
    std::string serial_src_name() const;

    /**
     * Write an attribute. Can assume that a_name and a_value are valid as per the
     * definition() object.
     * 
     * NOTE: This is mainly useful during deserialisation. This method has the following
     * restrictions.
     * label, compound
     */
    void set_attribute(const std::string & a_name, std::string a_value);

    /**
     * The number of datum children
     */
    unsigned int size_datum() const throw ()
    {
      return m_results.size ();
    }

    /**
     * The number of info children
     */
    unsigned int size_info() const throw ()
    {
      return m_info.size ();
    }

    /**
     * A reference to the mechanism object that created the contained results.
     */
    const std::string& target_mechanism() const throw ()
    {
      return m_mechanism;
    }

    /**
     * Set the mechanism label. Changing this once set is not usual.
     * 
     * \pre program.end_mech != program.find_mech (a_target) [TODO]
     */
    void target_mechanism(std::string a_target) throw ()
    {
      COMPCHEM_REQUIRE (not is_src () or is_open (), "can not add children to closed memo-src objects");
      m_mechanism.swap (a_target);
    }

    /**
     * Get the full path to the document/source that contains the current element 
     * (also see basic_datum::uri_path)
     */
    std::string uri_path() const;

    /**
     * Get the scheme used to connect to the document/source that contains 
     * the current element. (also see basic_datum::uri_scheme)
     */
    std::string uri_scheme() const;

    /**
     * The URI of the document (memo-src nodes)
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
    /**
     * Call the on_visit(memo) method of the visitor
     */
    void visit(visitor & a_visitor) const;

    /**
     * Trigger a memo-src document to write itself.
     * 
     * \pre is_src (and is_open?)
     */
    void write() const;

};

} // namespace data_scheme
#endif
