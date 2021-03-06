// ------------------------------------------------------------
// class: project_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/project_py.hpp"
#include "data_scheme/compound_ref.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/visitor.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

/**
 * Make a getter function that is nicer for python.
 */
info * project_py::get_information_py(project & a_self)

{
  return (a_self.has_information () ? &(a_self.information ()) : (info *)(0));
}

/**
 * Make a src object.
 */
void project_py::make_src(project & a_proj, const std::string& a_path)

{
  a_proj.make_src(a_path);
}

/**
 * Create the python definitions for the project data_scheme C++ class
 */
void project_py::pythonise()

{
  /// Pythonise project
  ///
  /// The pythonisation of the project C++ class.
  project::experiment_iterator (project::*project_begin_experiment)() = &project::begin_experiment;
  project::experiment_iterator (project::*project_end_experiment)() = &project::end_experiment;
  project::compound_ref_iterator (project::*project_begin_reference)() = &project::begin_compound_ref;
  project::compound_ref_iterator (project::*project_end_reference)() = &project::end_compound_ref;
  const std::string & (project::*get_project_label)() const = &project::label;
  // void (project::*set_project_label)(std::string) = &project::label;
  
  boost::python::class_< project, boost::python::bases< basic_datum_py >, boost::noncopyable >("project",
          "A project should be single consistent branch of the main research \n"
          "program. You should be able to combine the results of a project into \n"
          "a single meaningful report.",
          boost::python::no_init
          )
   .def("add_experiment", &project::add_experiment, boost::python::return_internal_reference<>(),
          "Create and add a new experiment object, using the passed text \nas the reference.")
   .def("add_info", &project::add_info, boost::python::return_internal_reference<>(),
          "Create and add a new information object, using the passed text \n"
          "as the name. This can only be done if no information already \n"
          "exists.")
   .def("add_outcome_label", &project::add_outcome_label,
          "Add the outcome label to the list of target outcomes.")
   .def("add_worker_label", &project::add_worker_label,
          "Add the label to the list of project workers.")
   .add_property("information", boost::python::make_function(project_py::get_information_py, boost::python::return_internal_reference<>()),
          "The information object, may be None.")
   .add_property("experiments", boost::python::range< boost::python::return_internal_reference<> >(project_begin_experiment, project_end_experiment),
          "The range of experiments that have been done.")
   .add_property("compound_references", boost::python::range< boost::python::return_internal_reference<> >(project_begin_reference, project_end_reference),
          "The range of references to compounds that should be imported into this project.")
   .def ("is_open", &project::is_open, "If is_src, then has the project body bean read?")
   .def ("is_src", &project::is_src, "Is the body of the project contained in a sub-file?")
   .add_property("label", boost::python::make_function(get_project_label, boost::python::return_value_policy< boost::python::return_by_value >()), /* set_project_label, */
          "The unique ID of the project. Note this can only be set if currently empty.")
   .def ("make_src", &project_py::make_src, "Convert a direct project into one where the body is in a sub-file?")
   .add_property("outcomes", boost::python::make_function(&project::outcomes, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The text list of target outcomes.")
   .add_property("workers", boost::python::make_function(&project::workers, boost::python::return_value_policy< boost::python::return_by_value >()),
          "The text list of the label's of workers.")
   ;

}


} // namespace data_scheme
