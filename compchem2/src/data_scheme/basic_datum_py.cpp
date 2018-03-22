// ------------------------------------------------------------
// class: basic_datum_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/basic_datum_py.hpp"
#include "data_scheme/text.hpp"
#include "data_scheme/visitor.hpp"

namespace data_scheme {

/**
 * Generate the pythonised version of basic_datum
 */
void basic_datum_py::pythonise()
{
  iterator (basic_datum::*basic_datum_begin)() = &basic_datum::begin;
  iterator (basic_datum::*basic_datum_end)() = &basic_datum::end;
  
  boost::python::class_< basic_datum_py, boost::noncopyable >("basic_datum",
          "Base class for all data_scheme components.",
          boost::python::no_init
          )
   .def("add_child", &basic_datum::add_child, boost::python::return_internal_reference<>(),
           "Create and append a child object to this object. The XML element name \n"
           "is the first argument and the key attribute value is the second. This \n"
           "should mainly be called by the deserialisation system as it will throw \n"
           "exceptions for invalid XML elements. Therefore, programs should use \n"
           "the 'add_?' methods of sub-types to add new children safely.")
   .def("add_comment", &basic_datum::add_comment, boost::python::return_internal_reference<>(),
           "Append an XML comment to this object.")
   .def("add_pi", &basic_datum::add_pi, boost::python::return_internal_reference<>(),
           "Append an XML processing instruction to this object.")
   .def("add_text", &basic_datum::add_text, boost::python::return_internal_reference<>(),
           "Append text to this object. It is the responsibility of the programmer \n"
           "to ensure that appending text is valid. Adding 'non-significant' white\n"
           "space is generally OK.")
   .add_property("children", boost::python::range< boost::python::return_internal_reference<> >(basic_datum_begin, basic_datum_end),
            "The complete ordered range of children objects.")
   .def("empty", &basic_datum::empty, "Does this object have children?")
   .def("get_attribute", &basic_datum_py::get_attribute_py, "Package method for getting attributes during serialisation")
   .def("has_attribute", &basic_datum_py::has_attribute_py, "Package method for getting attributes during serialisation")
   .def("set_attribute", &basic_datum::set_attribute, "Package method for setting attributes during deserialisation.")
   .def("visit", boost::python::pure_virtual(&basic_datum::visit),
             "Virtual function used by derived classes to call the appropriate \n"
             "on_visit method of the visitor class.")
   ;

}

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
basic_datum & basic_datum_py::add_child(const std::string & a_name, std::string & a_label) 
{
  COMPCHEM_ALWAYS(false, "Can not add children to a this object");
  // Dummy return value.
  return *this;
}


} // namespace data_scheme

