// ------------------------------------------------------------
// class: info_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/info_py.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Create the python definitions of the info data_scheme C++ class
 */
void info_py::pythonise()

{
  /// Pythonise info
  ///
  /// The pythonisation of the info C++ class.
  const std::string& (info::*get_info_name)() const = &info::name;
  void (info::*set_info_name)(std::string) = &info::name;
  info::info_iterator (info::*get_info_begin)() = &info::begin_info;
  info::info_iterator (info::*get_info_end)() = &info::end_info;
  
  boost::python::class_< info, boost::python::bases< basic_datum_py >, boost::noncopyable >("info",
          "This element contains text. The type attribute identifies what the \n"
          "text is. This element is intended to be a general mechanism for \n"
          "including arbitrary text markup.",
          boost::python::no_init
          )
   .def("add_info", &info::add_info, boost::python::return_internal_reference<>(),
          "Create, append and return an information child object, using the \n"
          "passed text as the 'name' of the new object")
   .add_property("name", boost::python::make_function(get_info_name, boost::python::return_value_policy< boost::python::return_by_value >()), set_info_name,
          "A label indicating the meaning of the content of this object.")
   .add_property("infos", boost::python::range (get_info_begin, get_info_end),
          "The range of information child objects, may be empty.")
    ;
  
  

}


} // namespace data_scheme
