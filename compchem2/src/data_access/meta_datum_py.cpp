// ------------------------------------------------------------
// class: meta_datum_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_access/meta_datum_py.hpp"
#include "data_access/meta_datum.hpp"

// Manual includes
#include <boost/python.hpp>
// -
namespace data_access {

/**
 * Create the python definition of the meta_datum data_access C++ class
 */
void meta_datum_py::pythonise()

{
  /// Pythonise the meta_datum class
  ///
  ///
  
  boost::python::class_< meta_datum, boost::noncopyable >("meta_datum",
          "Information about how a data_scheme class represents itself in an XML stream.",
          boost::python::no_init
          )
   .def("has_attr_name", &meta_datum::has_attr_name, "Is the arg one of our attribute labels?")
   .def("has_attributes", &meta_datum::has_attributes, "Do we have any attributes labels?")
   .def("has_child_tag", &meta_datum::has_child_tag, "Is the arg onr of our child tag data_access::names?")
   .def("empty_children", &meta_datum::empty_children, "Do we have any child tag data_access::names?")
   .def("valid_attribute", &meta_datum::valid_attribute
         , "Is the first arg in our attr list and the second a valid attribute value.")
   .add_property("attributes", boost::python::range< boost::python::return_value_policy< boost::python::return_by_value > >(&meta_datum::begin_attr, &meta_datum::end_attr)
         , "The range of attribute labels, may be empty.")
   .add_property("name", boost::python::make_getter(&meta_datum::name, boost::python::return_value_policy< boost::python::return_by_value >())
         , "The tag name of the XML element.")
   .add_property("key_attribute", boost::python::make_getter(&meta_datum::key_attribute, boost::python::return_value_policy< boost::python::return_by_value >())
         , "The key attribute required to create a new object. All objects\n"
           "provide this but it may not be critical for all types.")
   ;
  
  

}


} // namespace data_access
