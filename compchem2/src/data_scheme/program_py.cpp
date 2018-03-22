// ------------------------------------------------------------
// class: program_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/program_py.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/visitor.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/worker.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Make a getter function that is nicer for python.
 */
info * program_py::get_information_py(program & a_self)

{
  return (a_self.has_information () ? &(a_self.information ()) : (info *)(0));
}

/**
 * Constructor for program objects.
 */
boost::shared_ptr< program > program_py::init_program(const std::string& a_uri)

{
  boost::shared_ptr< program > Result (new data_scheme::program);
  Result->connect (a_uri);
  return Result;
}

/**
 * Create the python definitions of the program data_scheme C++ class
 */
void program_py::pythonise()

{
  /// Pythonise program
  ///
  /// The pythonisation of the program C++ class.
  program::compound_iterator (program::*program_begin_compound)() = &program::begin_compound;
  program::compound_iterator (program::*program_end_compound)() = &program::end_compound;
  program::mechanism_iterator (program::*program_begin_mechanism)() = &program::begin_mechanism;
  program::mechanism_iterator (program::*program_end_mechanism)() = &program::end_mechanism;
  program::project_iterator (program::*program_begin_project)() = &program::begin_project;
  program::project_iterator (program::*program_end_project)() = &program::end_project;
  program::worker_iterator (program::*program_begin_worker)() = &program::begin_worker;
  program::worker_iterator (program::*program_end_worker)() = &program::end_worker;
  
  boost::python::class_< program, boost::python::bases< basic_datum_py >, boost::noncopyable >("program",
          "A program of research is a broad category of interest. It is intended \n"
          "that the contents of a possible paper do not cross program boundaries, \n"
          "although several papers may be contained in a program.")
   .def("__init__", boost::python::make_constructor (&program_py::init_program),
          "Create a program using the database filename given. This\n"
          "is the only public constructor in the data_scheme library.")
   .def("add_compound", &program::add_compound, boost::python::return_internal_reference<>(),
          "Create and add a new compound child, using the passed argument as\n"
          "the compound's label.")
   .def("add_info", &program::add_info, boost::python::return_internal_reference<>(),
          "Create and add a new information child, using the passed argument as\n"
          "the info's name. This may only be done if no existing information\n"
          "object exists.")
   .def("add_mechanism", &program::add_mechanism, boost::python::return_internal_reference<>(),
          "Create and add a new mechanism child, using the passed argument as\n"
          "the mechanism's label.")
   .def("add_project", &program::add_project, boost::python::return_internal_reference<>(),
          "Create and add a new project child, using the passed argument as\n"
          "the project's label.")
   .def("add_project_src", &program::add_project_src, boost::python::return_internal_reference<>(),
          "Create and add a new project child, using the passed argument as\n"
          "the porject's database filename.")
   .def("add_worker", &program::add_worker, boost::python::return_internal_reference<>(),
          "Create and add a new worker child, using the passed argument as\n"
          "the worker's label.")
   .add_property("information", boost::python::make_function(&program_py::get_information_py, boost::python::return_internal_reference<>()),
          "The information object, may be None")
   .add_property("compounds", boost::python::range< boost::python::return_internal_reference<> >(program_begin_compound, program_end_compound),
          "The range of child compound objects, may be empty.")
   .add_property("mechanisms", boost::python::range< boost::python::return_internal_reference<> >(program_begin_mechanism, program_end_mechanism),
          "The range of child mechanism objects, may be empty.")
   .add_property("projects", boost::python::range< boost::python::return_internal_reference<> >(program_begin_project, program_end_project),
          "The range of child project objects, may be empty.")
   .add_property("workers", boost::python::range< boost::python::return_internal_reference<> >(program_begin_worker, program_end_worker),
          "The range of child worker objects, may only be empty for new program's.")
   .def ("write", &program::write, "Write the database")
    ;
  

}


} // namespace data_scheme
