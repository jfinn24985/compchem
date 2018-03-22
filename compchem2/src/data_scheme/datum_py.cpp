// ------------------------------------------------------------
// class: datum_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/datum_py.hpp"
#include "data_scheme/datum.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Create the python version of the datum C++ class
 */
void datum_py::pythonise()

{
  /// Pythonise datum
  ///
  /// The pythonisation of the datum C++ class.
  static const std::string& (datum::*get_datum_dim)() const = &datum::dim;
  static void (datum::*set_datum_dim)(std::string) = &datum::dim;
  static const std::string& (datum::*get_datum_format)() const = &datum::format;
  static void (datum::*set_datum_format)(std::string) = &datum::format;
  static const std::string& (datum::*get_datum_outcome)() const = &datum::outcome;
  static void (datum::*set_datum_outcome)(std::string) = &datum::outcome;
  static int (datum::*get_datum_ordinal)() const = &datum::ordinal;
  static void (datum::*set_datum_ordinal)(int) = &datum::ordinal;
  static const std::string& (datum::*get_datum_scale)() const = &datum::scale;
  static void (datum::*set_datum_scale)(std::string) = &datum::scale;
  datum::datum_iterator (datum::*datum_begin_datum)() = &datum::begin_datum;
  datum::datum_iterator (datum::*datum_end_datum)() = &datum::end_datum;
  
  boost::python::class_< datum, boost::python::bases< basic_datum_py >, boost::noncopyable >("datum",
          "This element contains data for a particular observable property of a \n"
          "formula (compound). It can contain any sort of data. Expected datum \n"
          "elements for chemical structures:\n"
          "'format': How the atoms are listed, such as atom+xyz or \n"
          "atom+length+angle+dihedral or a map from one atomlist onto another \n"
          "(for use with form-memo etc). eg. 'axyz' is atom label and x,y,z \n"
          "coordinates. 'alad' is atom label and length, angle, dihedral (eg \n"
          "z-matrix). 'map' is pairs of atom numbers that map one structure onto \n"
          "another. 'stdrep' is a well-defined representation that is understood \n"
          "by source/report plugins. 'cml' indicatescontent is in CML XML format. \n"
          "'lab' indicates the contents are laboratory quantities.\n"
          "'charge': The charge on the molecule\n"
          "'multiplicity': The multiplicity of the structure.",
          boost::python::no_init
          )
   .def("add_datum", &datum::add_datum, boost::python::return_internal_reference<>(), "Create a new datum object as a child of this object, with the \ngiven 'outcome' text")
   .add_property("dim", boost::python::make_function(get_datum_dim, boost::python::return_value_policy< boost::python::return_by_value >()), set_datum_dim, "A description of the dimensions of this datum")
   .add_property("data", boost::python::range< boost::python::return_internal_reference<> >(datum_begin_datum, datum_end_datum), "The datum children of this object")
   .def("empty_datum", &datum::empty_datum, "Are there next-level children?")
   .add_property("format", boost::python::make_function(get_datum_format, boost::python::return_value_policy< boost::python::return_by_value >()), set_datum_format, "Optional description of the format of the contents")
   .add_property("outcome", boost::python::make_function(get_datum_outcome, boost::python::return_value_policy< boost::python::return_by_value >()), set_datum_outcome, "The physical outcome this datum contains.")
   .def("has_ordinal", &datum::has_ordinal, "is the optional ordinal value set (ie not the default)?")
   .add_property("ordinal", get_datum_ordinal, set_datum_ordinal, "Optional ordinal value indicate the position of this child in a SET or\nLIST. This will return a default value unless has_ordinal is true.")
   .def("unset_ordinal", &datum::unset_ordinal, "Change the ordinal to the default value")
   .add_property("scale", boost::python::make_function(get_datum_scale, boost::python::return_value_policy< boost::python::return_by_value >()), set_datum_scale, "Optional description of scale of value. This should indicate a value to \nconvert the result to standard S.I. units for 'outcome'. For \nexample this could be 'kCal' for kilocalories or '10e6' for 'mega' \nsomething")
    ;
  

}


} // namespace data_scheme
