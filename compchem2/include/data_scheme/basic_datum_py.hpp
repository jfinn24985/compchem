#ifndef COMPCHEM_DATA_SCHEME_BASIC_DATUM_PY_HPP
#define COMPCHEM_DATA_SCHEME_BASIC_DATUM_PY_HPP

// ------------------------------------------------------------
// class: basic_datum_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/basic_datum.hpp"
#include <boost/python.hpp>

#include <string>

namespace data_scheme { class basic_text; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

/**
 * Wrap the virtual functions of basic_datum
 */
class basic_datum_py : public basic_datum, public boost::python::wrapper<basic_datum_py>
{
  public:
    /**
     * Convert the C++ function into a more python like function. This
     * returns whether there is an attribute value.
     */
    static bool has_attribute_py(const basic_datum & a_self, const std::string & a_key)
    {
      std::string l_dummy;
      return a_self.get_attribute (a_key, l_dummy);
    }
    

    /**
     * Convert the C++ function into a more python like function. You 
     * have to assume that an empty string indicates the attribute is
     * not set.
     */
    static std::string get_attribute_py(const basic_datum & a_self, const std::string & a_key)
    {
      std::string Result;
      a_self.get_attribute (a_key, Result);
      return Result;
    }

    void visit(visitor & a_visitor) const
    {
      this->get_override("visit")(a_visitor);
    }

    /**
     * Generate the pythonised version of basic_datum
     */
    static void pythonise();

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


};

} // namespace data_scheme
#endif
