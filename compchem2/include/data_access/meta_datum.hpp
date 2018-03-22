#ifndef COMPCHEM_META_DATUM_HPP
#define COMPCHEM_META_DATUM_HPP

// ------------------------------------------------------------
// class: meta_datum
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_access/names.hpp"
#include <set>
#include <string>
#include <boost/ptr_container/ptr_map.hpp>

#include <memory>

namespace data_access {

/**
 * Objects of this type contain an abstract definition of data_scheme type, which
 * are used by xml_persistence to deserialise XML objects.
 * 
 * It can:
 * Check for valid tag name.
 * Check for valid attribute name
 * Check for valid child name
 * 
 * It can _not_
 * Check for valid attribute content
 * Check for valid child order 
 */
class meta_datum
{
  public:
    typedef std::set< std::string >::const_iterator const_iterator;

    typedef boost::ptr_map<std::string, meta_datum> name_map_t;

    /**
     * An attribute name that is REQUIRED and key for this datum type. This
     * is used by the reader to select the second argument of the add_child
     * constructor.
     * 
     * It can be empty indicating no key attribute.
     */
    const std::string key_attribute;


  private:
    /**
     * The set of allowed attributes
     */
    std::set<std::string> m_attributes;

    /**
     * The names of allowed child elements
     */
    std::set<std::string> m_children;


  public:
    /**
     * The XML tag for this datum.
     */
    const std::string name;


  private:
    /**
     * The map of names to meta_datum objects.
     */
    static std::auto_ptr<name_map_t> s_map;


  public:
    /**
     * Get an iterator to the first attribute name.
     */
    const_iterator begin_attr() const
    {
      return m_attributes.begin ();
    }

    /**
     * Get the key attribute for the tag a_name.
     * 
     * I this returned the key_attribute then the reader would have to 
     * find the attribute and meta_datum would be independent of the
     * reader type.
     * 
     * \pre names::is_element_name (a_name) 
     */
    static const meta_datum & by_name(const std::string & a_name);

    /**
     * Get an iterator to the first attribute name.
     */
    const_iterator end_attr() const
    {
      return m_attributes.end ();
    }

    /**
     * Does this element have children tags?
     */
    bool empty_children() const throw ()
    {
      return m_children.empty ();
    }

    /**
     * Does this object allow the given attribute name.
     */
    bool has_attr_name(const std::string & a_name) const throw ()
    {
      return m_attributes.count (a_name) != 0;
    }

    /**
     * Can this element have attributes
     */
    bool has_attributes() const throw ()
    {
      return not m_attributes.empty ();
    }

    /**
     * Does this element allow children with the given tag?
     */
    bool has_child_tag(const std::string & a_name) const throw ()
    {
      return (0 != m_children.count (a_name));
    }
    


  private:
    /**
     * Initialise map with data_scheme types.
     */
    static void initialise_map();

    /**
     * Main constructor. The tag name for this object is a_name, the list of 
     * child tag names is a_child_list and the list of attribute names is 
     * a_attribute_list. The attribute name a_key is a key attribute (#REQUIRED)
     * for the element, it will automatically be added to a_attribute_list.
     * Lists are space separated list of words.
     * 
     * \pre names::is_element_name (a_name) and names::is_element_name (ALL in a_child_list) and names::is_attribute_name (ALL in a_attribute_list)
     * 
     * PROGRAM NOTE, List parsing is:
     * m_children.insert(istream_iterator<..>(istringstream(a_child_list)), istream_iterator<...>())
     */
    meta_datum(const std::string & a_name, const std::string & a_key, const std::string & a_child_list, const std::string & a_attribute_list);

    /**
     * no copy
     */
    meta_datum(const meta_datum & source);


  public:
    ~meta_datum() throw ();


  private:
    /**
     * no assign
     */
    meta_datum & operator=(const meta_datum & source);


  public:
    /**
     * Return if this is a valid attribute name, value pair. This checks that a_name
     * is a valid attribute label and that a_value is a valid attribute content. Note this
     * can also check that a coded form of a_value is an acceptable attribtue 
     * content.
     * 
     * \return has_attr_name (a_name)
     * \todo validate attribute value (a_value)
     */
    bool valid_attribute(const std::string & a_name, const std::string & a_value) const throw ()
    {
      return has_attr_name (a_name);
    }

  friend class test_meta_datum;

};

} // namespace data_access
#endif
