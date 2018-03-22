// ------------------------------------------------------------
// class: data_scheme_py_main
// $revision:$
// $log:$
// ------------------------------------------------------------

/**
 * Call the definitions for the individual data_scheme classes
 */
#include "data_scheme/data_scheme_py_main.hpp"
#include "data_scheme/text_py.hpp"
#include "data_scheme/compound_py.hpp"
#include "data_scheme/compound_ref_py.hpp"
#include "data_scheme/datum_py.hpp"
#include "data_scheme/experiment_py.hpp"
#include "data_scheme/filter_py.hpp"
#include "data_scheme/formula_py.hpp"
#include "data_scheme/identity_py.hpp"
#include "data_scheme/info_py.hpp"
#include "data_scheme/mechanism_py.hpp"
#include "data_scheme/memo_py.hpp"
#include "data_scheme/program_py.hpp"
#include "data_scheme/project_py.hpp"
#include "data_scheme/settings_py.hpp"
#include "data_scheme/visitor_py.hpp"
#include "data_scheme/worker_py.hpp"
#include "data_scheme/data_scheme_helper.hpp"

// Manual includes
#include "data_scheme/basic_datum_py.hpp"
#include <boost/python.hpp>
// -
namespace data_scheme {

bool data_scheme_py::get_auto_open_memo() 
{
  return data_scheme_helper::helper ().auto_open_memo ();
}

void data_scheme_py::set_auto_open_memo(bool a_proj) 
{
  data_scheme_helper::helper ().auto_open_memo (a_proj);
}

bool data_scheme_py::get_auto_project() 
{
  return data_scheme_helper::helper ().auto_project ();
}

void data_scheme_py::set_auto_project(bool a_proj) 
{
  data_scheme_helper::helper ().auto_project (a_proj);
}

const std::string & data_scheme_py::get_target_project() 
{
  return data_scheme_helper::helper ().target_project ();
}

void data_scheme_py::set_target_project(const std::string& a_proj) 
{
  data_scheme_helper::helper ().target_project (a_proj);
}

// This file defines the python wrappers for the data_scheme namespace
// into the data_scheme python module.
//
// * basic_datum (and wrap_basic_datum)
// * basic_text (and wrap_basic_text)
// * comment
// * compound
// * datum
// * experiment
// * filter
// * formula
// * identity
// * info
// * mechanism
// * memo
// * processing_instruction
// * program
// * project
// * settings
// * text
// * visitor
// * worker

BOOST_PYTHON_MODULE(data_scheme)
{

  basic_datum_py::pythonise ();
  basic_text_py::pythonise ();
  compound_py::pythonise ();
  compound_ref_py::pythonise ();
  datum_py::pythonise ();
  experiment_py::pythonise ();
  filter_py::pythonise ();
  formula_py::pythonise ();
  identity_py::pythonise ();
  info_py::pythonise ();
  mechanism_py::pythonise ();
  memo_py::pythonise ();
  program_py::pythonise ();
  project_py::pythonise ();
  settings_py::pythonise ();
  worker_py::pythonise ();
  visitor_py::pythonise ();

  boost::python::class_< data_scheme_py, boost::noncopyable >("helper",
        "Manage configuration for the data_scheme set of classes. Note that\n"
        "all instances of this object refer to the same object.",
        boost::python::init<>()
        )
   .add_property("target_project", boost::python::make_function(&data_scheme_py::get_target_project, boost::python::return_value_policy< boost::python::return_by_value >()), &data_scheme_py::set_target_project,
        "The project to use, default is 'default'.")
   .add_property("auto_open_memo", boost::python::make_function(&data_scheme_py::get_auto_open_memo, boost::python::return_value_policy< boost::python::return_by_value >()), &data_scheme_py::set_auto_open_memo,
        "When true will cause document reads to automatically open memo-src subfiles (default is 'false').")
   .add_property("auto_project", boost::python::make_function(&data_scheme_py::get_auto_project, boost::python::return_value_policy< boost::python::return_by_value >()), &data_scheme_py::set_auto_project,
        "When true will cause an attempt to automatically create missing projects (default is 'false').")
  ;  

}


} // namespace data_scheme
