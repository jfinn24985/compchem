#ifndef COMPCHEM_DATA_SCHEME_DATUM_HPP
#define COMPCHEM_DATA_SCHEME_DATUM_HPP

// ------------------------------------------------------------
// class: datum
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <map>
#include <boost/ptr_container/ptr_vector.hpp>
#include "data_scheme/basic_datum.hpp"
#include <string>

namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * This element contains data for a particular observable property of a formula(compound). 
 * It can contain any sort of data.
 * 
 * No extra methods are provided for building datum trees.  The methods available in dom::element should be sufficient (insert_before, replace_child, append_child etc)
 * 
 * Note: This relies on basic_datum to manage any textual content.
 */
class datum : public basic_datum {
  public:
    enum ordinal_type
    {
      NO_ORDINAL = -1/**
       * The value that indicates no ordinal value was set.
       */


    };

    typedef boost::ptr_vector< datum >::iterator datum_iterator;

    typedef boost::ptr_vector< datum >::const_iterator const_datum_iterator;


  private:
    /**
     * The parent element of this element
     */
    basic_datum & m_parent;

    /**
     * The label of the outcome that this datum contains.
     */
    std::string m_outcome;

    /**
     * Sub-content of this datum (if any)
     */
    boost::ptr_vector<datum> m_data;

    /**
     * The unit dimensions of the property. This is to allow for conversion between different
     * software. Default is a dimensionless value.
     * 
     * The @dim contains a comma separated list of dimension constants. They specify the
     * unit dimensions of the content, for  example velocity (m/s) would be "1,-1" and
     * force (N=kg m /s2) would be "1,-2,1". The order of dimensions is fixed. The list
     * need only extend to the last non-zero element, so volume would be "3" not
     * "3,0,0,0...".
     */
    std::string m_dim;

    /**
     * Optional format that the data can take, for use by software in reading the data.
     * 
     * How the atoms are listed, such as atom+xyz or atom+length+angle+dihedral or a map from one
     * atomlist onto another (for use with chem-notation etc).
     * format labels mean: "axyz" is atom label and x,y,z coordinates. "alad" is atom
     * label and length, angle, dihedral (eg z-matrix). "map" is pairs of atom numbers
     * that map one structure onto another. "stdrep" is a well-defined
     * representation that is understood by source/report plugins. "cml" indicates
     * content is in CML XML format. "lab" indicates the contents are laboratory
     * quantities. "free" indicates that a "freeformat" element will describe the
     * data somehow.
     */
    std::string m_format;

    /**
     * The (optional) position of this element in a "set" or "list" element.
     * The non-value is NO_ORDINAL (-1). All ordinal values must be larger
     * than this value.
     */
    int m_ordinal;

    /**
     * A optional scaling for the datum value.  This can be SI recognised letters ("k"=kilo,
     * "n"=nano etc), an arbitrary number ("1E-6" or "2345.67") or an non-SI unit name (eg
     * "hartree").  It is recommended that unit names are not used, instead the constant that
     * scales the non-SI unit to the SI unit be used.
     * 
     * This value is a multiplier of the content. Some programs specify data as a
     * ratio of two integers, here the divisor can be specified as the scale using "1/....."
     */
    std::string m_scale;


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
     * Create a basic_datum child of this node with the type tagname a_name.
     * Throw an error  if the a_name is not correct. Throw an error if no 
     * children are allowed.  Returns a reference to the new child object.
     * 
     * \return /= NULL
     */
    datum & add_datum(std::string a_label);

    datum_iterator begin_datum()
    {
      return m_data.begin ();
    }

    const_datum_iterator begin_datum() const
    {
      return m_data.begin ();
    }

    /**
     * Construct a new datum object, swallow a_outcome
     * 
     * \post a_outcome.empty
     */
    datum(basic_datum & a_parent, std::string & a_outcome)
    : basic_datum ()
    , m_parent (a_parent)
    , m_outcome ()
    , m_data ()
    , m_dim ()
    , m_format ()
    , m_ordinal (NO_ORDINAL)
    , m_scale ()
    {
      m_outcome.swap (a_outcome);
    }


  private:
    /**
     * no Copy constructor
     */
    datum(const datum & source);


  public:
    ~datum() throw ()
    {}
    /**
     * Get the unit dimensions of the property. This is to allow for conversion between different
     * software. Default is a dimensionless value (see 'scale' for information on converting non-SI
     * units).
     * 
     * The @dim contains a comma separated list of dimension constants. They specify the
     * unit dimensions of the content, for  example velocity (m/s) would be "1,-1" and
     * force (N=kg m /s2) would be "1,-2,1". The order of dimensions is fixed. The list
     * need only extend to the last non-zero element, so volume would be "3" not
     * "3,0,0,0...". It can also contain textual representations of the SI unit.
     */
    const std::string& dim() const throw ()
    {
      return m_dim;
    }

    /**
     * Set the unit dimensions of the property. This is to allow for conversion between different
     * software. Default is a dimensionless value (see 'scale' for information on converting non-SI
     * units). \see dim() for more details.
     */
    void dim(std::string a_val) throw ()
    {
       m_dim.swap (a_val);
    }

    bool empty_datum() const throw ()
    {
      return m_data.empty ();
    }

    datum_iterator end_datum()
    {
      return m_data.end ();
    }

    const_datum_iterator end_datum() const
    {
      return m_data.end ();
    }

    /**
     * Optional format that the data can take, for use by software in reading the data.
     * 
     * How the atoms are listed, such as atom+xyz or atom+length+angle+dihedral or a map from one
     * atomlist onto another (for use with chem-notation etc).
     * format labels mean: "axyz" is atom label and x,y,z coordinates. "alad" is atom
     * label and length, angle, dihedral (eg z-matrix). "map" is pairs of atom numbers
     * that map one structure onto another. "stdrep" is a well-defined
     * representation that is understood by source/report plugins. "cml" indicates
     * content is in CML XML format. "lab" indicates the contents are laboratory
     * quantities. "free" indicates that a "freeformat" element will describe the
     * data somehow.
     */
    const std::string& format() const throw ()
    {
      return m_format;
    }

    /**
     * Optional format that the data can take, for use by software in reading the data.
     * 
     * How the atoms are listed, such as atom+xyz or atom+length+angle+dihedral or a map from one
     * atomlist onto another (for use with chem-notation etc).
     * format labels mean: "axyz" is atom label and x,y,z coordinates. "alad" is atom
     * label and length, angle, dihedral (eg z-matrix). "map" is pairs of atom numbers
     * that map one structure onto another. "stdrep" is a well-defined
     * representation that is understood by source/report plugins. "cml" indicates
     * content is in CML XML format. "lab" indicates the contents are laboratory
     * quantities. "free" indicates that a "freeformat" element will describe the
     * data somehow.
     */
    void format(std::string a_str) throw ()
    {
      m_format.swap (a_str);
    }

    /**
     * Add attributes to the XML element
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;

    /**
     * Does this datum have an ordinal set?
     */
    bool has_ordinal() const throw ()
    {
      return m_ordinal > NO_ORDINAL;
    }

    /**
     * The name of the observable property.  Note that this name is 
     * context-specific.  This means that two datum objects with the 
     * same otucome attribute represent the same property type if their
     * contexts are the same.  Conversely, datum objects in different 
     * contexts but with the same outcome label may or may not 
     * represent the same property.
     * 
     * Examples names and one possible interpretation
     * charge        The integral charge of a molecule
     * multiplicity  The electronic multiplicity of a molecule. 
     */
    const std::string & outcome() const throw ()
    {
      return m_outcome;
    }

    /**
     * The name of the observable property.  Note that this name is 
     * context-specific.  This means that two datum objects with 
     * the same name attribute represent the same property type if 
     * their contexts are the same.  Conversely, datum objects in 
     * different contexts may or may not represent the same property.
     * 
     * charge      The charge on the molecule
     * multiplicity The multiplicity of the structure. 
     */
    void outcome(std::string a_name) throw ()
    {
      m_outcome.swap (a_name);
    }


  private:
    /**
     * no assign
     */
    datum& operator=(const datum & source);


  public:
    /**
     * Return a pointer to the parent object, may be NULL.
     */
    const basic_datum & parent() const throw ()
    {
      return m_parent;
    }

    /**
     * Get the datum ordinal.  This defines the datum object position when the parent's hint
     * type is SET or LIST.
     * 
     * \pre has_ordinal
     * \post result >= 0
     */
    int ordinal() const
    {
      COMPCHEM_REQUIRE(has_ordinal (), "Can not get ordinal when not present.");
      return m_ordinal;
    }

    /**
     * Set the datum ordinal value. \see ordinal() for more details
     * 
     * \pre a_val >= 0
     */
    void ordinal(int a_val)
    {
     COMPCHEM_REQUIRE(a_val > NO_ORDINAL, "Can not set ordinal to zero or a negative number.");
     m_ordinal = a_val;
    }

    /**
     * Remove a_datum from our list of children.
     * 
     * \pre a_datum.parent = this
     */
    void remove_child(datum & a_datum);

  private:
    /**
     * Remove a memo object.
     * 
     * \pre a_datum.parent = this
     */
    void remove_priv(datum & a_datum);


  public:
    /**
     * A optional scaling for the datum value.  When this is not set the datum is
     * assumed to be in the standard SI units. This can be SI recognised letters ("k"=kilo,
     * "n"=nano etc), an arbitrary number ("1E-6" or "2345.67") or an non-SI unit name (eg
     * "hartree").  It is recommended that unit names are not used, instead the constant that
     * scales the non-SI unit to the SI unit be used.
     * 
     * This value is a multiplier of the content. Some programs specify data as a
     * ratio of two integers, here the divisor can be specified as the scale using "1/....."
     */
    const std::string & scale() const throw ()
    {
      return m_scale;
    }

    /**
     * A optional scaling for the datum value.  \see scale() for more details.
     */
    void scale(std::string a_val) throw ()
    {
      m_scale.swap (a_val);
    }

    /**
     * The key name for this class during serialisation.
     */
    std::string serial_name() const;

    /**
     * Write an attribute.  Throw an error if arguments are incorrect or if no 
     * attributes are allowed.
     */
    void set_attribute(const std::string & a_name, std::string a_value);

    /**
     * The number of datum children of the datum object
     */
    unsigned int size_datum() const throw ()
    {
      return m_data.size ();
    }

    /**
     * Set ordinal to non-value
     */
    void unset_ordinal() throw ()
    {
     m_ordinal = NO_ORDINAL;
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
