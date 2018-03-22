// ------------------------------------------------------------
// class: filter_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/filter_py.hpp"
#include "data_scheme/filter.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Create the python definitions of the filter data_scheme C++ class
 */
void filter_py::pythonise()

{
  /// Pythonise filter
  /// 
  /// The python definition of the filter C++ class
  const std::string& (filter::*get_filter_href)() const = &filter::href;
  void (filter::*set_filter_href)(std::string) = &filter::href;
  
  boost::python::class_< filter, boost::python::bases< basic_datum_py >, boost::noncopyable >("filter",
          "This object contains information about an external filter that is \n"
          "available for operating on files created by the parent mechanism \n"
          "to import outcomes into the DB.",
          boost::python::no_init
          )
   .def("add_outcome_label", &filter::add_outcome_label, "Add an outcome label to the list of outcomes.")
   .add_property("label", boost::python::make_function(&filter::label, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The unique ID of this label, allowing it to be referred to from \nother objects.")
   .add_property("href", boost::python::make_function(get_filter_href, boost::python::return_value_policy< boost::python::return_by_value >()), set_filter_href,
          "The URI of the filter. This is generally a filename in the one of \nthe plugin filter directories.")
   .add_property("outcomes", boost::python::make_function(&filter::outcomes, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The list of outcomes that this filter can provide when applied to \nthe mechanism result file.")
    ;
  
  

}


} // namespace data_scheme
