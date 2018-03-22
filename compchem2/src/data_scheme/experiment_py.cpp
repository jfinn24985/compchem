// ------------------------------------------------------------
// class: experiment_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/experiment_py.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Create the python definitions of the experiment data_scheme class.
 */
void experiment_py::pythonise()

{
  /// Pythonise experiment
  ///
  /// The pythonisation of the experiment C++ class.
  boost::gregorian::date (experiment::*get_experiment_date_start)() const = &experiment::date_start;
  void (experiment::*set_experiment_date_start)(const boost::gregorian::date&) = &experiment::date_start;
  boost::gregorian::date (experiment::*get_experiment_date_end)() const = &experiment::date_end;
  void (experiment::*set_experiment_date_end)(const boost::gregorian::date&) = &experiment::date_end;
  unsigned int (experiment::*get_experiment_priority)() const = &experiment::priority;
  void (experiment::*set_experiment_priority)(unsigned int) = &experiment::priority;
  experiment::memo_iterator (experiment::*experiment_begin_memo)() = &experiment::begin_memo;
  experiment::memo_iterator (experiment::*experiment_end_memo)() = &experiment::end_memo;
  experiment::info_iterator (experiment::*experiment_begin_info)() = &experiment::begin_info;
  experiment::info_iterator (experiment::*experiment_end_info)() = &experiment::end_info;
      
  boost::python::class_< experiment, boost::python::bases< basic_datum_py >, boost::noncopyable >("experiment",
          "The outcome of a set of work done by the chemist/worker. Also the \n"
          "description of what was done. The description of what was performed \n"
          "is contained in the info elements that are direct children of the \n"
          "experiment. The filtered results of any results is contained in \n"
          "datum elements that are children of a memo element.",
          boost::python::no_init
          )
   .def("add_info", &experiment::add_info, boost::python::return_internal_reference<>(),
          "Create and append an information child object. Use the passed arg \n"
          "as the info name.")
   .def("add_memo", &experiment::add_memo, boost::python::return_internal_reference<>(),
          "Create and append a memo child object, using the arg as the memo \nlabel.")
   .def("add_memo_src", &experiment::add_memo_src, boost::python::return_internal_reference<>(),
          "Create and append a memo child object, using the arg as the new \nmemo database file location.")
   .add_property("date_start", get_experiment_date_start, set_experiment_date_start,
          "Get the date the experiment started on. May be 'unknown'")
   .add_property("date_end", get_experiment_date_end, set_experiment_date_end,
          "Get the date the experiment ended on, the default is start_date.")
   .add_property("info", boost::python::range< boost::python::return_internal_reference<> >(experiment_begin_info, experiment_end_info),
          "Get the range of information children.")
   .add_property("memos", boost::python::range< boost::python::return_internal_reference<> >(experiment_begin_memo, experiment_end_memo),
          "Get the range of memo children")
   .add_property("priority", get_experiment_priority, set_experiment_priority,
          "The importance of the results of this experiment. This may be used \n"
          "when outputting results to select the most important result set.")
   .add_property("reference", boost::python::make_function(&experiment::reference, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The unique ID of this experiment. It may, for example, refer to a \n"
          "unique page in a physical lab-book.\n")
    ;
  
  

}


} // namespace data_scheme
