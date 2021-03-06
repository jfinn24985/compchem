// ------------------------------------------------------------
// class: memo_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/memo_py.hpp"
#include "data_scheme/visitor.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/datum.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/info.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Make a src object.
 */
void memo_py::make_src(memo & a_memo, const std::string& a_path)

{
  a_memo.make_src(a_path);
}

/**
 * Create python definition of the memo data_scheme C++ class
 */
void memo_py::pythonise()

{
  /// Pythonise memo
  ///
  /// The pythonisation of the memo C++ class.
  const std::string& (memo::*get_memo_href)() const = &memo::href;
  void (memo::*set_memo_href)(std::string) = &memo::href;
  const std::string& (memo::*get_memo_label)() const = &memo::label;
  // void (memo::*set_memo_label)(std::string) = &memo::label;
  const std::string& (memo::*get_memo_mechanism)() const = &memo::target_mechanism;
  void (memo::*set_memo_mechanism)(std::string) = &memo::target_mechanism;
  memo::datum_iterator (memo::*memo_begin_datum)() = &memo::begin_datum;
  memo::datum_iterator (memo::*memo_end_datum)() = &memo::end_datum;
  memo::info_iterator (memo::*memo_begin_info)() = &memo::begin_info;
  memo::info_iterator (memo::*memo_end_info)() = &memo::end_info;
  
  boost::python::class_< memo, boost::python::bases< basic_datum_py >, boost::noncopyable >("memo",
          "This contains important settings and results. The settings \n"
          "give details of the operation that was performed to generate \n"
          "the results. It should provide details specific to this \n"
          "experiment and include all critical input parameters. The \n"
          "results should be key results cached from the result file. \n"
          "For example, in computational chemistry this would mean \n"
          "details of the method, basis-set and calculation type.",
          boost::python::no_init
          )
   .def("add_datum", &memo::add_datum, boost::python::return_internal_reference<>(),
        "Create and add a datum child, using passed text as datum outcome.")
   .def("add_filter_label", &memo::add_filter_label,
        "Append text as a filter label to the list of filters.")
   .def("add_info", &memo::add_info, boost::python::return_internal_reference<>(),
        "Create and add an information child, using passed text as the info\nname.")
   .add_property("compound_label", boost::python::make_function(&memo::compound_label, boost::python::return_value_policy< boost::python::return_by_value >()),
        "The label of the compound this refers to.")
   .add_property("formula_label", boost::python::make_function(&memo::formula_label, boost::python::return_value_policy< boost::python::return_by_value >()),
        "The label of the formula, of the compound, this refers to.")
   .add_property("filters", boost::python::make_function(&memo::filters, boost::python::return_value_policy< boost::python::return_by_value >()),
        "The comma separated list of filters that where applied to generate this memo.")
   .add_property("href", boost::python::make_function(get_memo_href, boost::python::return_value_policy< boost::python::return_by_value >()), set_memo_href,
        "The filename of the result file this memo we generated from.")
   .def ("is_open", &memo::is_open, "If is_src, then has the memo body bean read?")
   .def ("is_src", &memo::is_src, "Is the body of the memo contained in a sub-file?")
   .add_property("label", boost::python::make_function(get_memo_label, boost::python::return_value_policy< boost::python::return_by_value >()), /* set_memo_label, */
        "The ID of this memo object. Note this can only be set if currently empty.")
   .def ("make_src", &memo_py::make_src, "Convert a direct memo into one where the body is in a sub-file?")
   .add_property("target_mechanism", boost::python::make_function(get_memo_mechanism, boost::python::return_value_policy< boost::python::return_by_value >()), set_memo_mechanism,
        "The mechanism that generated the result file.")
   .add_property("data", boost::python::range(memo_begin_datum, memo_end_datum),
        "The range of datum children.")
   .add_property("info", boost::python::range(memo_begin_info, memo_end_info),
        "The range of info children.")
   ;
  

}


} // namespace data_scheme
