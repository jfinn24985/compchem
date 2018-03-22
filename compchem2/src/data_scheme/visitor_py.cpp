// ------------------------------------------------------------
// class: visitor_py
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/visitor_py.hpp"
#include "data_scheme/text.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/compound_ref.hpp"
#include "data_scheme/datum.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/filter.hpp"
#include "data_scheme/formula.hpp"
#include "data_scheme/identity.hpp"
#include "data_scheme/info.hpp"
#include "data_scheme/mechanism.hpp"
#include "data_scheme/memo.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/settings.hpp"
#include "data_scheme/worker.hpp"
#include "data_scheme/basic_datum.hpp"

namespace data_scheme {

void visitor_py::visit_comment(const comment & a_datum) 
{
  boost::python::override func = this->get_override ("visit_comment");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_comment (a_datum);
}

void visitor_py::def_visit_comment(const comment & a_datum) 
{
  this->visitor::visit_comment (a_datum);
}

void visitor_py::visit_compound(const compound & a_datum) 
{
  boost::python::override func = this->get_override ("visit_compound");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_compound (a_datum);
}

void visitor_py::def_visit_compound(const compound & a_datum) 
{
  this->visitor::visit_compound (a_datum);
}

void visitor_py::visit_compound_ref(const compound_ref & a_datum) 
{
  boost::python::override func = this->get_override ("visit_compound_ref");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_compound_ref (a_datum);
}

void visitor_py::def_visit_compound_ref(const compound_ref & a_datum) 
{
  this->visitor::visit_compound_ref (a_datum);
}

void visitor_py::visit_datum(const datum & a_datum) 
{
  boost::python::override func = this->get_override ("visit_datum");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_datum (a_datum);
}

void visitor_py::def_visit_datum(const datum & a_datum) 
{
  this->visitor::visit_datum (a_datum);
}

void visitor_py::visit_experiment(const experiment & a_datum) 
{
  boost::python::override func = this->get_override ("visit_experiment");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_experiment (a_datum);
}

void visitor_py::def_visit_experiment(const experiment & a_datum) 
{
  this->visitor::visit_experiment (a_datum);
}

void visitor_py::visit_filter(const filter & a_datum) 
{
  boost::python::override func = this->get_override ("visit_filter");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_filter (a_datum);
}

void visitor_py::def_visit_filter(const filter & a_datum) 
{
  this->visitor::visit_filter (a_datum);
}

void visitor_py::visit_formula(const formula & a_datum) 
{
  boost::python::override func = this->get_override ("visit_formula");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_formula (a_datum);
}

void visitor_py::def_visit_formula(const formula & a_datum) 
{
  this->visitor::visit_formula (a_datum);
}

void visitor_py::visit_identity(const identity & a_datum) 
{
  boost::python::override func = this->get_override ("visit_identity");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_identity (a_datum);
}

void visitor_py::def_visit_identity(const identity & a_datum) 
{
  this->visitor::visit_identity (a_datum);
}

void visitor_py::visit_info(const info & a_datum) 
{
  boost::python::override func = this->get_override ("visit_info");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_info (a_datum);
}

void visitor_py::def_visit_info(const info & a_datum) 
{
  this->visitor::visit_info (a_datum);
}

void visitor_py::visit_mechanism(const mechanism & a_datum) 
{
  boost::python::override func = this->get_override ("visit_mechanism");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_mechanism (a_datum);
}

void visitor_py::def_visit_mechanism(const mechanism & a_datum) 
{
  this->visitor::visit_mechanism (a_datum);
}

void visitor_py::visit_memo(const memo & a_datum) 
{
  boost::python::override func = this->get_override ("visit_memo");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_memo (a_datum);
}

void visitor_py::def_visit_memo(const memo & a_datum) 
{
  this->visitor::visit_memo (a_datum);
}

void visitor_py::visit_pi(const processing_instruction & a_datum) 
{
  boost::python::override func = this->get_override ("visit_pi");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_pi (a_datum);
}

void visitor_py::def_visit_pi(const processing_instruction & a_datum) 
{
  this->visitor::visit_pi (a_datum);
}

void visitor_py::visit_program(const program & a_datum) 
{
  boost::python::override func = this->get_override ("visit_program");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_program (a_datum);
}

void visitor_py::def_visit_program(const program & a_datum) 
{
  this->visitor::visit_program (a_datum);
}

void visitor_py::visit_project(const project & a_datum) 
{
  boost::python::override func = this->get_override ("visit_project");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_project (a_datum);
}

void visitor_py::def_visit_project(const project & a_datum) 
{
  this->visitor::visit_project (a_datum);
}

void visitor_py::visit_settings(const settings & a_datum) 
{
  boost::python::override func = this->get_override ("visit_settings");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_settings (a_datum);
}

void visitor_py::def_visit_settings(const settings & a_datum) 
{
  this->visitor::visit_settings (a_datum);
}

void visitor_py::visit_text(const text & a_datum) 
{
  boost::python::override func = this->get_override ("visit_text");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_text (a_datum);
}

void visitor_py::def_visit_text(const text & a_datum) 
{
  this->visitor::visit_text (a_datum);
}

void visitor_py::visit_worker(const worker & a_datum) 
{
  boost::python::override func = this->get_override ("visit_worker");
  if (func)
    func (a_datum);
  else
    this->visitor::visit_worker (a_datum);
}

void visitor_py::def_visit_worker(const worker & a_datum) 
{
  this->visitor::visit_worker (a_datum);
}

void visitor_py::visit_basic_datum(const basic_datum & a_datum) 
{
  this->get_override ("visit_basic_datum")(a_datum);
}

/**
 * Pythonise the visitor class
 */
void visitor_py::pythonise()

{
  /// Pythonise visitor
  ///
  /// This is the python version of the visitor class
  
  boost::python::class_< visitor_py, boost::noncopyable >("visitor",
          "A visitor object is used to operate on a tree of data_scheme\n"
          "types.  It passes itself to each object which then calls the\n"
          "appropriate on_visit_? method.\n"
          "The default implementation of these methods is to call the \n"
          "on_visit_basic_datum method.",
          boost::python::init<>()
          )
   .def("visit_basic_datum", boost::python::pure_virtual (&visitor::visit_basic_datum),
        "Operate on the generic basic_datum type. This is also the\n"
        "default method if the derived class has not specified a\n"
        "more specific method. This is also pure virtual so must be\n"
        "defined in derived classes.")
   .def("visit_comment", &visitor::visit_comment, &visitor_py::def_visit_comment,
        "Operate on comment objects.")
   .def("visit_compound", &visitor::visit_compound, &visitor_py::def_visit_compound,
        "Operate on compound objects.")
   .def("visit_compound_ref", &visitor::visit_compound_ref, &visitor_py::def_visit_compound_ref,
        "Operate on compound objects.")
   .def("visit_datum", &visitor::visit_datum, &visitor_py::def_visit_datum,
        "Operate on datum objects.")
   .def("visit_experiment", &visitor::visit_experiment, &visitor_py::def_visit_experiment,
        "Operate on experiment objects.")
   .def("visit_filter", &visitor::visit_filter, &visitor_py::def_visit_filter,
        "Operate on filter objects.")
   .def("visit_formula", &visitor::visit_formula, &visitor_py::def_visit_formula,
        "Operate on formula objects.")
   .def("visit_identity", &visitor::visit_identity, &visitor_py::def_visit_identity,
        "Operate on identity objects.")
   .def("visit_info", &visitor::visit_info, &visitor_py::def_visit_info,
        "Operate on info objects.")
   .def("visit_mechanism", &visitor::visit_mechanism, &visitor_py::def_visit_mechanism,
        "Operate on mechanism objects.")
   .def("visit_memo", &visitor::visit_memo, &visitor_py::def_visit_memo,
        "Operate on memo objects.")
   .def("visit_pi", &visitor::visit_pi, &visitor_py::def_visit_pi,
        "Operate on processing_instruction objects.")
   .def("visit_program", &visitor::visit_program, &visitor_py::def_visit_program,
        "Operate on program objects.")
   .def("visit_project", &visitor::visit_project, &visitor_py::def_visit_project,
        "Operate on project objects.")
   .def("visit_settings", &visitor::visit_settings, &visitor_py::def_visit_settings,
        "Operate on settings objects.")
   .def("visit_text", &visitor::visit_text, &visitor_py::def_visit_text,
        "Operate on text objects.")
   .def("visit_worker", &visitor::visit_worker, &visitor_py::def_visit_worker,
        "Operate on worker objects.")
  ;

}

/**
 * Dtor (does nothing in this class)
 */
visitor_py::~visitor_py() 
{}


} // namespace data_scheme
