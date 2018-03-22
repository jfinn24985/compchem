#ifndef COMPCHEM_DATA_SCHEME_COMPOUND_HPP
#define COMPCHEM_DATA_SCHEME_COMPOUND_HPP

// ------------------------------------------------------------
// class: compound
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
#include <string>
#include <memory>

namespace data_scheme { class formula; } 
namespace data_scheme { class identity; } 
namespace data_scheme { class info; } 
namespace data_scheme { class program; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * A base-type for 'compound' type objects
 */
class compound : public basic_datum {
  public:
    /**
     * Iterator for formula list.
     */
    typedef boost::ptr_vector< formula >::iterator formula_iterator;

    /**
     * Const iterator for formula list.
     */
    typedef boost::ptr_vector< formula >::const_iterator const_formula_iterator;

    /**
     * Iterator for formula list.
     */
    typedef boost::ptr_vector< identity >::iterator identity_iterator;

    /**
     * Const iterator for formula list.
     */
    typedef boost::ptr_vector< identity >::const_iterator const_identity_iterator;


  private:
    /**
     * The category list
     */
    std::string m_categories;

    /**
     * The identities of this compound
     */
    boost::ptr_vector<identity> m_identity;

    /**
     * The (possible) information node for this compound.
     */
    std::auto_ptr<info> m_info;

    /**
     * List of formulae this compound has
     */
    boost::ptr_vector<formula> m_instances;

    /**
     * The compound label.
     */
    std::string m_label;

    /**
     * The parent program for this compound
     */
    program & m_parent;


  public:
    /**
     * Create a basic_datum child of this node with the type tagname a_name.
     * Throw an error not an allowed child.  Returns a reference to the new 
     * child object.
     */
    basic_datum & add_child(const std::string & a_name, std::string & a_label);

    /**
     * Create and insert a formula element. Return a pointer to the created formula.
     * 
     * \pre find_formula(a_label) = end_formula
     * \post return /= nul
     */
    formula & add_formula(std::string a_label);

    /**
     * Create and insert a identity element. Return a pointer to the created formula.
     * 
     * \pre find_identity(a_label) = end_identity
     * \post return /= nul
     */
    identity & add_identity(std::string a_label);

    /**
     * Create and add an information object. Return the object.
     * 
     * \pre not has_information
     * \post return /= nul
     */
    info & add_info(std::string a_label);

    /**
     * A space delimited set of category IDs that allow formulae 
     * to be grouped in meaningful ways.  Order is not
     * important, though priority can be flagged by ending an ID 
     * with the ":" and a number (lower number is higher priority).  
     * For example if you are studying a reaction with several
     * steps you would identify each step with an ID, and if you 
     * are making a comparison across substituents then they 
     * could be IDed.  It would then be possible for a report to
     * generate a table of reaction v. substituent.
     */
    const std::string& categories() const throw ()
    {
      return m_categories;
    }

    /**
     * A space delimited set of category IDs that allow formulae to be grouped in meaningful ways.  Order is not
     * important, though priority can be flagged by ending an ID with the ":" and a number (lower
     * number is higher priority).  For example if you are studying a reaction with several
     * steps you would identify each step with an ID, and if you are making a comparison across
     * substituents then they could be IDed.  It would then be possible for a report to
     * generate a table of reaction v. substituent.
     */
    void add_category_label(const std::string& a_cat_set);
    /**
     * Get the vector of formulae.
     */
    formula_iterator begin_formula()
    {
      return m_instances.begin ();
    }

    /**
     * Get the vector of formulae.
     */
    const_formula_iterator begin_formula() const
    {
      return m_instances.begin ();
    }

    /**
     * Access the list of identities.
     */
    identity_iterator begin_identity()
    {
      return m_identity.begin ();
    }

    /**
     * Access the list of identities.
     */
    const_identity_iterator begin_identity() const
    {
      return m_identity.begin ();
    }

    /**
     * Construct a new compound object, swallow a_label
     * 
     * \post a_label.empty
     */
    compound(program & a_parent, std::string & a_label);


  private:
    /**
     * no copy
     */
    compound(const compound & source);


  public:
    ~compound() throw ();

    /**
     * Are there formula children?
     */
    bool empty_formula() const
    {
      return m_instances.empty ();
    }

    /**
     * Are there 'identity' children
     */
    bool empty_identity() const
    {
      return m_identity.empty ();
    }

    /**
     * Get an iterator to the one-past-last formula child
     */
    formula_iterator end_formula()
    {
      return m_instances.end ();
    }

    /**
     * Get a constant iterator to the one-past-last formula child
     */
    const_formula_iterator end_formula() const
    {
      return m_instances.end ();
    }

    /**
     * Access the list of identities.
     */
    identity_iterator end_identity()
    {
      return m_identity.end ();
    }

    /**
     * Access the list of identities.
     */
    const_identity_iterator end_identity() const
    {
      return m_identity.end ();
    }

    /**
     * Search for a formula with the given label
     */
    formula_iterator find_formula(const std::string & a_label);

    /**
     * Search for a formula with the given label
     */
    const_formula_iterator find_formula(const std::string & a_label) const;

    /**
     * Add attributes to element writer.
     * 
     * No default implementation.
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

    /**
     * Test if an information object exists.
     */
    bool has_information() const throw ()
    {
      return NULL != m_info.get ();
    }

    /**
     * Get the information content.
     * 
     * \pre has_information ()
     */
    info& information()
    {
      COMPCHEM_REQUIRE(has_information (), "Cannot get non-existent information object.");
      return *m_info.get ();
    }

    /**
     * Get the information content.
     * 
     * \pre has_information ()
     */
    const info& information() const
    {
      COMPCHEM_REQUIRE(has_information (), "Cannot get non-existent information object.");
      return *m_info.get ();
    }

    const std::string& label() const throw ()
    {
      return m_label;
    }


  private:
    /**
     * no assignment
     */
    compound & operator=(const compound & source);


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
    void remove_child(formula & a_datum);
    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(identity & a_datum);

  private:
    /**
     * Remove a formula object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(formula & a_datum);

    /**
     * Remove a identity object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(identity & a_datum);


  public:
    /**
     * The XML element name for this datum.
     */
    std::string serial_name() const;

    /**
     * Write an attribute.  Throw an error if arguments are incorrect or if no 
     * attributes are allowed.
     */
    void set_attribute(const std::string & a_name, std::string a_value);

    /**
     * How many formula children are there?
     */
    unsigned int size_formula() const throw ()
    {
      return m_instances.size ();
    }

    /**
     * How many identity children are there?
     */
    unsigned int size_identity() const throw ()
    {
      return m_identity.size ();
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
