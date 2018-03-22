// ------------------------------------------------------------
// class: compound_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/compound_py.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/identity.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/formula.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Make a getter function that is nicer for python.
 */
info * compound_py::get_information_py(compound & a_self)

{
  return (a_self.has_information () ? &(a_self.information ()) : (info *)(0));
}

/**
 * Create python definitions of the compound C++ class
 */
void compound_py::pythonise()

{
  /// Pythonise compound
  ///
  /// The pythonisation of the compound C++ class.
  compound::formula_iterator (compound::*compound_begin_formula)() = &compound::begin_formula;
  compound::formula_iterator (compound::*compound_end_formula)() = &compound::end_formula;
  compound::identity_iterator (compound::*compound_begin_identity)() = &compound::begin_identity;
  compound::identity_iterator (compound::*compound_end_identity)() = &compound::end_identity;
      
  boost::python::class_< compound, boost::python::bases< basic_datum_py >, boost::noncopyable >("compound",
          "This is a container for information about what a chemist would \n"
          "call a compound. \n"
          "Note: this can include various different geometries/formulations \n"
          "for the same chemical compound. \n"
          "Note: The relationship between a compound and a program is \n"
          "currently bi-directional. Ideally a compound may belong to \n"
          "several programs but this is not currently supported.",
          boost::python::no_init
          )
   .def("add_category_label", &compound::add_category_label,
          "Append a compound category label. Categories are labels that \n"
          "allow compounds to be grouped.")
   .def("add_formula", &compound::add_formula, boost::python::return_internal_reference<>(),
          "Create a new formula object as a child of this object, using the\n"
          "passed text as the formula label. Returns the new object.")
   .def("add_identity", &compound::add_identity, boost::python::return_internal_reference<>(),
          "Create a new identity object as a child of this object, using the\n"
          "passed text as the identity type. Returns the new object.")
   .def("add_info", &compound::add_info, boost::python::return_internal_reference<>(),
          "Create a new information object as a child of this object, using the\n"
          "passed text as the info name. Returns the new object.")
   .add_property("categories", boost::python::make_function(&compound::categories, boost::python::return_value_policy< boost::python::return_by_value >()),
          "Text containing a list of category labels. These labels allow \ncompounds to be grouped.")
   .add_property("formulae", boost::python::range< boost::python::return_internal_reference<> >(compound_begin_formula, compound_end_formula),
          "An iterable collection of formula objects contained in this compound. \nMay be empty")
   .add_property("identities", boost::python::range< boost::python::return_internal_reference<> >(compound_begin_identity, compound_end_identity),
          "An iterable collection of identity objects contained in this compound. \nMay be empty.")
   .add_property("information", boost::python::make_function(&compound_py::get_information_py, boost::python::return_internal_reference<>()),
          "The information object of this compound. (may be None)")
   .add_property("label", boost::python::make_function(&compound::label, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The unique ID of this compound.")
    ;
  

}


} // namespace data_scheme
