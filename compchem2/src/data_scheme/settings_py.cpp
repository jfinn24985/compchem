// ------------------------------------------------------------
// class: settings_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/settings_py.hpp"
#include "data_scheme/settings.hpp"
#include "data_scheme/visitor.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/datum.hpp"
#include "data_scheme/mechanism.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Make a getter function that is nicer for python.
 */
datum * settings_py::get_data_py(settings & a_self)

{
  return (a_self.has_data () ? &(a_self.data ()) : (datum *)(0));
}

/**
 * Make a getter function that is nicer for python.
 */
info * settings_py::get_information_py(settings & a_self)

{
  return (a_self.has_information () ? &(a_self.information ()) : (info *)(0));
}

/**
 * Create the python definition of the settings data_scheme C++ class
 */
void settings_py::pythonise()

{
  /// Pythonise settings
  ///
  /// The pythonisation of the settings C++ class.
  
  boost::python::class_< settings, boost::python::bases< basic_datum_py >, boost::noncopyable >("settings",
          "A object for configuring/tailoring mechanisms to produce certain \noutcomes.",
          boost::python::no_init
          )
   .def("add_info", &settings::add_info, boost::python::return_internal_reference<>(),
          "Create and add a new info object, using the passed text as \n"
          "the name. This can only be done if no information already\nexists.")
   .def("add_datum", &settings::add_datum, boost::python::return_internal_reference<>(),
          "Create and add a new datum object, using the passed text as \n"
          "the outcome. This can only be done if no data already\nexists.")
   .def("add_filter_label", &settings::add_filter_label,
          "Add the label of a filter to the list.")
   .def("add_outcome_label", &settings::add_outcome_label,
          "Add the outcome to the list.")
   .add_property("data", boost::python::make_function(settings_py::get_data_py, boost::python::return_internal_reference<>()),
          "The datum object, may be None.")
   .def("filter_list", &settings::filter_list, boost::python::return_value_policy< boost::python::return_by_value >(),
          "The textual list of filter labels that are used with these settings.")
   .add_property("information", boost::python::make_function(settings_py::get_information_py, boost::python::return_internal_reference<>()),
          "The informaiont object, may be None.")
   .add_property("label", boost::python::make_function(&settings::label, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The unique ID of this object.")
    ;
  

}


} // namespace data_scheme
