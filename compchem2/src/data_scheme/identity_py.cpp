// ------------------------------------------------------------
// class: identity_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/identity_py.hpp"
#include "data_scheme/identity.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Create the python definitions of the identity data_scheme C++ class
 */
void identity_py::pythonise()

{
  /// Pythonise identity
  /// 
  /// The python definition of the identity C++ class
  boost::python::class_< identity, boost::python::bases< basic_datum_py >, boost::noncopyable >("identity",
          "A compound is universally unique. Within a program it is given \n"
          "an arbitrary ID label. However that are standards for uniquely \n"
          "identifying a compound across programs, eg iupac-name, CAS \n"
          "number etc. The content of this element contain this information \n"
          "for use in matching compounds across programs.",
          boost::python::no_init
          )
   .add_property("identity_type", boost::python::make_function(&identity::identity_type, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The key to what the identity object contains.")
    ;
  

}


} // namespace data_scheme
