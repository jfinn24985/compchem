// ------------------------------------------------------------
// class: formula_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/formula_py.hpp"
#include "data_scheme/formula.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/identity.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Make a getter function that is nicer for python.
 */
info * formula_py::get_information_py(formula & a_self)

{
  return (a_self.has_information () ? &(a_self.information ()) : (info *)(0));
}

/**
 * Create the python definitions of the formula data_scheme C++ class
 */
void formula_py::pythonise()

{
  /// Pythonise formula
  ///
  /// The pythonisation of the formula C++ class.
  formula::identity_iterator (formula::*formula_begin_identity)() = &formula::begin_identity;
  formula::identity_iterator (formula::*formula_end_identity)() = &formula::end_identity;
  
  boost::python::class_< formula, boost::python::bases< basic_datum_py >, boost::noncopyable >("formula",
          "Formula object in data_scheme components. This specifies a \n"
          "particular geometry of a molecule. Here formula means a particular \n"
          "set of atom positions which may be described empirically, such as \n"
          "the 'ground state' for experimental results. It can also describe \n"
          "the product produced under a common set of conditions/procedure or \n"
          "a material made to a certain composition.\n"
          "The relationship between a compound and formula is 1-to-many. This \n"
          "means that a formula can only have one parent compound.",
          boost::python::no_init)
   .def("add_identity", &formula::add_identity, boost::python::return_internal_reference<>(),
          "Create an identity object as a child of this object, using \n"
          "the passed text as the 'identity type' for the child.")
   .def("add_info", &formula::add_info, boost::python::return_internal_reference<>(),
          "Create an information object as a child of this object, using \n"
          "the passed text as the 'name' for the child.")
   .add_property("identities", boost::python::range< boost::python::return_internal_reference<> >(formula_begin_identity, formula_end_identity),
          "The range of identity child objects.")
   .add_property("information", boost::python::make_function(&formula_py::get_information_py, boost::python::return_internal_reference<>()),
          "The information child object, may be None.")
   .add_property("label", boost::python::make_function(&formula::label, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The 'label' text of this formula.")
   ;
  

}


} // namespace data_scheme
