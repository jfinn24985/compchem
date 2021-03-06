// ------------------------------------------------------------
// class: worker_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/worker_py.hpp"
#include "data_scheme/worker.hpp"
#include "data_scheme/visitor.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/info.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Create the python definition of the worker data_scheme C++ class
 */
void worker_py::pythonise()

{
  /// Pythonise worker
  ///
  /// The pythonisation of the worker C++ class.
  const std::string& (worker::*get_worker_label)() const = &worker::label;
  // void (worker::*set_worker_label)(std::string) = &worker::label;
  worker::info_iterator (worker::*worker_begin_info)() = &worker::begin_info;
  worker::info_iterator (worker::*worker_end_info)() = &worker::end_info;
  
  boost::python::class_< worker, boost::python::bases< basic_datum_py >, boost::noncopyable >("worker",
          "A person doing work. This allowes us to assign people who performed\n"
          "the work in programs and projects.",
          boost::python::no_init
          )
   .def("add_info", &worker::add_info, boost::python::return_internal_reference<>(),
        "Create and add a new info object, using the passed text as \nthe info's name.")
   .add_property("info", boost::python::range(worker_begin_info, worker_end_info),
        "The range of info objects. This should not be empty.")
   .add_property("label", boost::python::make_function(get_worker_label, boost::python::return_value_policy< boost::python::return_by_value >()), /* set_worker_label, */
        "The unique ID of this worker.")
    ;
  
  

}


} // namespace data_scheme
