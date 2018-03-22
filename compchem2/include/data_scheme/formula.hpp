#ifndef COMPCHEM_DATA_SCHEME_FORMULA_HPP
#define COMPCHEM_DATA_SCHEME_FORMULA_HPP

// ------------------------------------------------------------
// class: formula
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

namespace data_scheme { class identity; } 
namespace data_scheme { class info; } 
namespace data_scheme { class compound; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * This specifies a particular geometry of a molecule. Here formula means a particular
 * set of atom positions which may be described empirically, such as the "ground
 * state" for experimental results. It can also describe the product produced under a 
 * common set of conditions/procedure or a material made to a certain composition.
 * 
 * The relationship between a compound and formula is 1-to-many.  This means that a
 * formula can only have one parent compound.
 */
class formula : public basic_datum {
  public:
    typedef boost::ptr_vector< identity >::iterator identity_iterator;

    typedef boost::ptr_vector< identity >::const_iterator const_identity_iterator;


  private:
    /**
     * The proper name for this formula.
     */
    boost::ptr_vector<identity> m_identity;

    /**
     * The info for this formula
     */
    std::auto_ptr<info> m_info;

    /**
     * The formula label attribute
     */
    std::string m_label;

    /**
     * The compound this formula is a member of (parent)
     */
    compound & m_moiety;


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
     * Create and return an identity object with a_label as a child of this object.
     * 
     * \pre find_identity(a_label) = end_identity
     * \return /= NULL
     */
    identity & add_identity(std::string a_label);
    /**
     * Add an info object as a child of this object.
     * 
     * \pre not has_information
     * \return /= NULL
     */
    info & add_info(std::string a_label);
    /**
     * Return an iterator to the first identity child object.
     */
    identity_iterator begin_identity()
    {
      return m_identity.begin ();
    }

    /**
     * Return an iterator to the first identity child object.
     */
    const_identity_iterator begin_identity() const
    {
      return m_identity.begin ();
    }

    /**
     * Return an iterator to the first identity child object.
     */
    bool empty_identity() const throw ()
    {
      return m_identity.empty ();
    }

    /**
     * Return an iterator to the first identity child object.
     */
    identity_iterator end_identity()
    {
      return m_identity.end ();
    }

    /**
     * Return an iterator to the first identity child object.
     */
    const_identity_iterator end_identity() const
    {
      return m_identity.end ();
    }

    /**
     * Construct a new formula object with a_parent and a_label. 
     * 
     * swallows a_label
     * \post a_label.empty
     */
    formula(compound & a_parent, std::string & a_label);


  private:
    /**
     * no copy
     */
    formula(const formula & source);


  public:
    ~formula() throw ();

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
     * Get the parent compound of this formula
     */
    const compound & moiety() const throw ()
    {
      return m_moiety;
    }


  private:
    /**
     * no assign
     */
    formula & operator=(const formula & source);

  public:
    /**
     * Return a pointer to the parent object.
     */
    const basic_datum & parent() const throw ();

    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(identity & a_datum);

  private:
    /**
     * Remove a identity object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(identity & a_datum);


  public:
    /**
     * The key for this class during serialisation.
     */
    std::string serial_name() const;

    /**
     * Throw an error as program has no attributes.
     */
    virtual void set_attribute(const std::string & a_name, std::string a_value);

    /**
     * Return an iterator to the first identity child object.
     */
    unsigned int size_identity() const throw ()
    {
      return m_identity.size ();
    }

    void visit(visitor & a_visitor) const;


  protected:
    /**
     * Called when an object of a derived class will be removed.
     * 
     * throw exception if a_datum is not a child of derived class.
     */
    virtual void virt_remove_child(basic_datum * a_datum);

};

} // namespace data_scheme
#endif
