#ifndef COMPCHEM_DATA_SCHEME_BASIC_DATUM_HPP
#define COMPCHEM_DATA_SCHEME_BASIC_DATUM_HPP

// ------------------------------------------------------------
// class: basic_datum
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>

namespace data_scheme { class basic_text; } 
namespace data_scheme { class comment; } 
namespace data_scheme { class processing_instruction; } 
namespace data_scheme { class text; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * This class is the base class for all data_scheme components. It
 * defines methods used by the system to read from and write to XML
 * documents.
 * 
 * It's purpose is to provide the abstract interface to allow an XML Reader
 * and a data factory object to parse an XML document into basic_datum 
 * derived objects. Additionally, the interface should allow different derived
 * types to provide for scanning and querying of the XML document.
 * 
 * Its second function is to handle all text related elements (XML Text,
 * Comment and PI elements) directly.  It uses virt_[add|remove]_text
 * methods to inform descendents when these elements are added or
 * removed. For simply replace all XML Text elements you can use 
 * set_text, for finer control you need to play with iterators.
 */
class basic_datum
{
  public:
    /**
     * Iterator type for the ordered list of children.
     */
    typedef std::vector< basic_datum * >::const_iterator const_iterator;

    /**
     * Iterator type for the ordered list of children.
     */
    typedef std::vector< basic_datum * >::iterator iterator;


  private:
    /**
     * The basic textual elements in this datum.  These elements are
     * often textual elements that appears in the XML document but 
     * do not form part of the logical document structure. (eg 
     * whitespace, comments and PIs)
     */
    boost::ptr_vector<basic_text> m_base_elements;

    /**
     * The list of children in document order.  This is includes the logical
     * objects and XML elements such as comments, whitespace etc.
     */
    std::vector<basic_datum *> m_children;


  public:
    /**
     * add_child creates a new object corresponding to a_name and appends it
     * to the list of children.
     * 
     * Note: add_child always appends objects to list. It should therefore only be 
     * used by deserialisation classes.
     * 
     * throw is a_name is not a valid child.
     * base-class action is to throw an error!
     * 
     * \post a_label.empty
     * 
     * \return /= NULL
     */
    virtual basic_datum & add_child(const std::string& a_name, std::string & a_label);

    /**
     * Create a comment child of this node and append to child list. Throw an error if no 
     * children are allowed.  Returns a reference to the new child object.
     * 
     * \return /= NULL
     * \final
     */
    comment & add_comment(std::string a_value)
    {
      return add_comment_swallow (a_value);
    }

    /**
     * Create a comment object and append to child list.  Swallow 
     * a_value. Throw an error if no children are allowed.  Returns a 
     * reference to the new child object.
     * 
     * Swallows text!
     * \post a_value.empty
     * 
     * \return /= NULL
     */
    virtual comment & add_comment_swallow(std::string & a_value);

    /**
     * Create a processing_instruction and append it to list of children. Throw 
     * an error if no children are allowed.  Returns a reference to the new child 
     * object.
     * 
     * \return /= NULL
     * \final
     */
    processing_instruction & add_pi(std::string a_name, std::string a_value)
    {
      return add_pi_swallow (a_name, a_value);
    }

    /**
     * Create a processing_instruction and append it to list of children. Throw 
     * an error if no children are allowed.  Returns a reference to the new child 
     * object. Swallows a_value text!
     * 
     * \post a_value.empty
     * 
     * \return /= NULL
     */
    virtual processing_instruction & add_pi_swallow(const std::string & a_name, std::string & a_value);

    /**
     * Create a text object and append it to list of children. Throw 
     * an error if no children are allowed.  Returns a reference to the new child 
     * object.
     * 
     * \return /= NULL
     * \final
     */
    text & add_text(std::string a_value)
    {
      return add_text_swallow (a_value);
    }

    /**
     * Create a text object and append it to list of children. Throw 
     * an error if no children are allowed.  Returns a reference to the new child 
     * object. Swallows a_value text!
     * 
     * \post a_value.empty
     * 
     * \return /= NULL
     */
    virtual text & add_text_swallow(std::string & a_value);


  protected:
    /**
     * This method is used to append a_new ptr to the list of children.  
     */
    void append_child(basic_datum & a_new)
    {
      m_children.push_back (&a_new);
    }


  public:
    basic_datum();


  private:
    /**
     * no copy
     */
    basic_datum(const basic_datum & source);


  public:
    virtual ~basic_datum() throw ();

    /**
     * The first child element of this datum
     */
    const_iterator begin() const
    {
      return m_children.begin ();
    }

    /**
     * The first child element of this datum
     */
    iterator begin()
    {
      return m_children.begin ();
    }

    /**
     * Does this datum have any children?
     */
    bool empty() const throw ()
    {
      return m_children.empty ();
    }

    /**
     * The first child element of this datum
     */
    const_iterator end() const
    {
      return m_children.end ();
    }

    /**
     * The first child element of this datum
     */
    iterator end()
    {
      return m_children.end ();
    }


  protected:
    /**
     * Remove a_item from child list.  This is called automatically in 
     * basic_datum::remove.
     */
    void erase(iterator a_item)
    {
      m_children.erase (a_item);
    }


  public:
    /**
     * Return an iterator to the given child ptr or end() if not found.
     */
    const_iterator find(const basic_datum *const a_ptr) const
    {
      return std::find (begin (), end (), a_ptr);
    }

    /**
     * Return an iterator to the given child ptr or end() if not found.
     */
    iterator find(const basic_datum *const a_ptr)
    {
      return std::find (begin (), end (), a_ptr);
    }

    /**
     * Get attribute value. If the object has an attribute value corresponding to 
     * a_name then it is placed in a_value and true is returned. Otherwise false
     * is returned and a_value is unchanged.
     * 
     * \post return=false implies a_value.pre === a_value.post
     */
    virtual bool get_attribute(const std::string & a_name, std::string & a_value) const;


  protected:
    /**
     * This method is used to add a_new ptr to the list of 
     * children after a_pre.  During deserialisation this is
     * handled automatically by the add_child method.  Therefore
     * this is only necessary when a derived classes creates
     * a new object internally.
     * 
     * \pre (untested) a_pre in [begin (), end ()]
     */
    void insert_child(iterator a_pre, basic_datum & a_new)
    {
      m_children.insert (a_pre, &a_new);
    }

    /**
     * This method is used to add a_new ptr to the list of 
     * children after a_pre.  During deserialisation this is
     * handled automatically by the add_child method.  Therefore
     * this is only necessary when a derived classes creates
     * a new object internally.
     * 
     * \pre (untested) a_pre in [begin (), end ()]
     */
    void insert_child(const basic_datum & a_pre, basic_datum & a_new)
    {
      m_children.insert (find (&a_pre), &a_new);
    }


  private:
    /**
     * no assign
     */
    basic_datum & operator=(const basic_datum & source);


  public:
    /**
     * Remove the given iterator from the children list.  
     * 
     * If the removed object was owned by the basic_datum base class object 
     * then virt_remove_text will be called.
     * 
     * \pre (unchecked) a_item in [begin, end)
     * \post a_item is invalid.
     */
    void remove(iterator a_item);

    /**
     * The XML element definition for this datum.
     */
    virtual std::string serial_name() const = 0;

    /**
     * Write an attribute.  Throw an error if arguments are incorrect or if no 
     * attributes are allowed.
     * 
     * This does not guarrantee that the following will hold
     * a.set_attribute ("A", "B") -> "B" = a.get_attribute ("A")
     * in the case where "A" is a key_attribute and is read-only.
     * Currently in this case the attempt to set the attribute may
     * be silently ignored. (TODO: should not be silent, should 
     * test that "B" is equal to current value or throw.)
     */
    virtual void set_attribute(const std::string & a_name, std::string a_value) = 0;

    unsigned int size() const throw ()
    {
      return m_children.size ();
    }


  protected:
    /**
     * Called when an object of a derived class will be removed.
     * 
     * throw exception if a_datum is not a child of derived class.
     */
    virtual void virt_remove_child(basic_datum * a_datum);


  public:
    virtual void visit(visitor & a_visitor) const = 0;


};

} // namespace data_scheme
#endif
