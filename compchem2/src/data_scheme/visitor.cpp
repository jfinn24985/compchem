// ------------------------------------------------------------
// class: visitor
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "data_scheme/visitor.hpp"
#include "data_scheme/basic_datum.hpp"
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

namespace data_scheme {

/**
 * Operator on comment objects.
 */
void visitor::visit_comment(const comment & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on compound objects.
 */
void visitor::visit_compound(const compound & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on compound objects.
 */
void visitor::visit_compound_ref(const compound_ref & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on datum objects.
 */
void visitor::visit_datum(const datum & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on experiment objects.
 */
void visitor::visit_experiment(const experiment & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on filter objects.
 */
void visitor::visit_filter(const filter & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on formula objects.
 */
void visitor::visit_formula(const formula & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on identity objects.
 */
void visitor::visit_identity(const identity & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on info objects.
 */
void visitor::visit_info(const info & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on mechanism objects.
 */
void visitor::visit_mechanism(const mechanism & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on memo objects.
 */
void visitor::visit_memo(const memo & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on processing_instruction objects.
 */
void visitor::visit_pi(const processing_instruction & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on program objects.
 */
void visitor::visit_program(const program & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on project objects.
 */
void visitor::visit_project(const project & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on settings objects.
 */
void visitor::visit_settings(const settings & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on text objects.
 */
void visitor::visit_text(const text & a_datum) 
{
  visit_basic_datum (a_datum);
}

/**
 * Operate on worker objects.
 */
void visitor::visit_worker(const worker & a_datum) 
{
  visit_basic_datum (a_datum);
}


} // namespace data_scheme
