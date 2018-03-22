// ------------------------------------------------------------
// class: mechanism_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/mechanism_py.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_scheme/visitor.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/filter.hpp"
#include "data_scheme/settings.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/project.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Make a getter function that is nicer for python.
 */
info * mechanism_py::get_information_py(mechanism & a_self)

{
  return (a_self.has_information () ? &(a_self.information ()) : (info *)(0));
}

/**
 * Create the python definition of the mechanism data_scheme C++ class
 */
void mechanism_py::pythonise()

{
  /// Pythonise mechanism
  ///
  /// The pythonisation of the mechanism C++ class.
  mechanism::filter_iterator (mechanism::*mechanism_begin_filter)() = &mechanism::begin_filter;
  mechanism::filter_iterator (mechanism::*mechanism_end_filter)() = &mechanism::end_filter;
  mechanism::settings_iterator (mechanism::*mechanism_begin_settings)() = &mechanism::begin_settings;
  mechanism::settings_iterator (mechanism::*mechanism_end_settings)() = &mechanism::end_settings;
  
  boost::python::class_< mechanism, boost::python::bases< basic_datum_py >, boost::noncopyable >("mechanism",
          "This contain information about something that produces results. It \n"
          "can be a computer program or a piece of lab equipemnt. The content \n"
          "should be information that is common to every time this 'mechanism' \n"
          "is referred to. This might be the bibliographic reference for a programme \n"
          "or the manufacturers address and model number. Set up parameters may be \n"
          "included if they are common to every usage (such as detector type). Usage \n"
          "specific information is contained in memo elements.\n" 
          "NOTE The contents of this element can also refer to a set of published \n"
          "results.",
          boost::python::no_init
          )
   .def("add_info", &mechanism::add_info, boost::python::return_internal_reference<>(),
          "Create and add an information object to this object, using the passed\n"
          "text as the information name. Can only be done when there is no \n"
          "current information object.")
   .def("add_filter", &mechanism::add_filter, boost::python::return_internal_reference<>(),
          "Create and add a new filter object, using the passed text as the \n"
          "filter label.")
   .def("add_setting", &mechanism::add_setting, boost::python::return_internal_reference<>(),
          "Create and add a new settings child, using the passed text as the \nsettings label.")
   .add_property("information", boost::python::make_function(&mechanism_py::get_information_py, boost::python::return_internal_reference<>()),
          "The information object for this mechanism, may be None.")
   .add_property("label", boost::python::make_function(&mechanism::label, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The unique ID of this mechanism.")
   .add_property("filters", boost::python::range(mechanism_begin_filter, mechanism_end_filter),
          "The range of filter children.")
   .add_property("settings", boost::python::range(mechanism_begin_settings, mechanism_end_settings),
          "The range of settings children.")
    ;
  
  

}


} // namespace data_scheme
