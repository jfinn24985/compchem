// ------------------------------------------------------------
// class: compound_ref_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/compound_ref_py.hpp"
#include "data_scheme/compound_ref.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Create python definition of the memo data_scheme C++ class
 */
void compound_ref_py::pythonise()

{
  /// Pythonise compound_ref
  ///
  /// The pythonisation of the compound_ref C++ class.
  
  boost::python::class_< compound_ref, boost::python::bases< basic_datum_py >, boost::noncopyable >("compound_ref",
          "This contains reference information about compounds and \n"
          "formulae that should be imported into the containing project.\n",
          boost::python::no_init
          )
   .def("add_filter_label", &compound_ref::add_formula_label,
        "Append text as a formula label to the list of formulae.")
   .add_property("compound_label", boost::python::make_function(&compound_ref::compound_label, boost::python::return_value_policy< boost::python::return_by_value >()),
        "The label of the compound this refers to.")
   .add_property("formula", boost::python::make_function(&compound_ref::formulae, boost::python::return_value_policy< boost::python::return_by_value >()),
        "A set of labels of formulae, of the compound, this refers to.")
   ;
  

}


} // namespace data_scheme
