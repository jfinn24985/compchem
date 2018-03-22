// ------------------------------------------------------------
// class: data_access_py_main
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_access/data_access_py_main.hpp"
#include "data_access/xml_access_helper.hpp"
#include "data_access/meta_datum_py.hpp"

// Manual includes
// #include <boost/python/module.hpp>
// #include <boost/python/def.hpp>
// #include <boost/python/to_python_converter.hpp>
// -
namespace data_access {

// This file defines the python wrappers for the data_scheme namespace
// into the data_scheme python module.
//
// * meta_datum
// * xml_file
// * xml_write_visitor

BOOST_PYTHON_MODULE(data_access)
{
  bool (xml_access_helper::*get_auto_fix)() const = &xml_access_helper::auto_fix;
  void (xml_access_helper::*set_auto_fix)(bool) = &xml_access_helper::auto_fix;

  boost::python::class_< xml_access_helper, boost::noncopyable >("xml_access_helper",
        "Manage configuration of the XML read/write classes.",
        boost::python::no_init
        )
   .add_static_property("helper", boost::python::make_function(&xml_access_helper::helper, boost::python::return_internal_reference<>()), "Get the data_access configuration management object.")
   .def("add_directory", &xml_access_helper::add_directory
       , "Add a directory to search for broken links.")
   .add_property("auto_fix", get_auto_fix, set_auto_fix
       , "Should we attempt to automatically fix broken links?")
   ;

  meta_datum_py::pythonise ();
}


} // namespace data_access
