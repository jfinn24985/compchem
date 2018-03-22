// ------------------------------------------------------------
// class: project_cfm_report
// $revision:$
// $log:$
// ------------------------------------------------------------

#define NO_LOG

#include "report/project_cfm_report.hpp"
#include "data_scheme/compound.hpp"
#include "data_scheme/compound_ref.hpp"
#include "data_scheme/experiment.hpp"
#include "data_scheme/formula.hpp"
#include "data_scheme/program.hpp"
#include "data_scheme/project.hpp"
#include "data_scheme/scoped_open.hpp"
#include "data_scheme/memo.hpp"
#include "data_access/xml_access_helper.hpp"
#include "data_access/xml_writer.hpp"

namespace report {

static bool compare_pair (const project_cfm_report::key_type & lhs, const project_cfm_report::key_type & rhs)
{
  return lhs.first < rhs.first or (lhs.first == rhs.first and lhs.second < rhs.second);
}/**
 * Add the referenced compound/formula(e). This inserts an entry
 * into a_compound_map with a nul for the memo ptr (iff no entry
 * already exists).
 */
void project_cfm_report::add_reference(const data_scheme::compound_ref & a_ref, project_cfm_report::compound_map_type & a_compound_map) const 
{
  // Iterate over formula list of program/compound or a_ref.formulae list.
  if (a_ref.formulae ().empty ())
  {
    data_scheme::program::const_compound_iterator l_comp (a_ref.section ().site ().find_compound (a_ref.compound_label ()));
    COMPCHEM_CHECK (l_comp != a_ref.section ().site ().end_compound ()
                    , "Invalid compound_ref that does not refer to an existing compound.");
    // Use all formulae of compound
    for (data_scheme::compound::const_formula_iterator l_formula_iter (l_comp->begin_formula ())
         ; l_formula_iter != l_comp->end_formula ()
         ; ++l_formula_iter)
    {
      const key_type l_key (a_ref.compound_label (), l_formula_iter->label ());
      compound_map_type::iterator l_cursor (a_compound_map.find (l_key));
      if (l_cursor == a_compound_map.end ())
      {
        a_compound_map.insert (compound_map_type::value_type (l_key, NULL));
      }
    }
  }
  else
  {
    // We assume that the compound/formula pairs are valid here
    boost::tokenizer<> l_tokens (a_ref.formulae ());
    for (boost::tokenizer<>::iterator l_formula_label (l_tokens.begin ())
         ; l_formula_label  != l_tokens.end ()
         ; ++l_formula_label)
    {
      const key_type l_key (a_ref.compound_label (), *l_formula_label);
      compound_map_type::iterator l_cursor (a_compound_map.find (l_key));
      if (l_cursor == a_compound_map.end ())
      {
        a_compound_map.insert (compound_map_type::value_type (l_key, NULL));
      }
    }
  }
  

}

/**
 * Scans the parent project for compound/formula/memos.
 * 
 * On the first pass it searches through the compound_ref and 
 * memo objects for formulae references that are added along 
 * with the corresponding compounds. If the source of the formula
 * reference was a memo object it is also added.
 * 
 * On the second pass the entire document is searched for memo 
 * objects for the referenced compound/formulae. 
 */
void project_cfm_report::generate_map(const data_scheme::project & a_parent, project_cfm_report::compound_map_type & a_compound_map) const 
{
  ///////////
  // Pass one
  
  // Add project's direct memos
  for (data_scheme::project::const_experiment_iterator l_expt_iter (a_parent.begin_experiment ())
       ; l_expt_iter != a_parent.end_experiment ()
       ; ++l_expt_iter)
  {
    for (data_scheme::experiment::const_memo_iterator l_memo_iter (l_expt_iter->begin_memo ())
         ; l_memo_iter != l_expt_iter->end_memo ()
         ; ++l_memo_iter)
    {
      const key_type l_key (l_memo_iter->compound_label (), l_memo_iter->formula_label ());
      DLOG("Adding memo with compound label=\"");DLOG(l_key.first);DLOG("\" and formula=\"");DLOG(l_key.second);DLOG("\"\n");
      a_compound_map.insert(compound_map_type::value_type (l_key, &(*l_memo_iter)));
    }
  }
  
  // Add compound references
  for (data_scheme::project::const_compound_ref_iterator l_ref_iter (a_parent.begin_compound_ref ()); l_ref_iter != a_parent.end_compound_ref (); ++l_ref_iter)
  {
    add_reference (*l_ref_iter, a_compound_map);
  }
  
  ///////////
  // Pass two
  
  // This tries to add the memos from compound_refs.
  for (data_scheme::program::const_project_iterator l_project_iter (a_parent.site ().begin_project ())
       ; l_project_iter != a_parent.site ().end_project ()
       ; ++l_project_iter)
  {
    if (l_project_iter->label () != a_parent.label ()) // Only check other projects
    {
      for (data_scheme::project::const_experiment_iterator l_expt_iter (l_project_iter->begin_experiment ())
           ; l_expt_iter != l_project_iter->end_experiment ()
           ; ++l_expt_iter)
      {
        for (data_scheme::experiment::const_memo_iterator l_memo_iter (l_expt_iter->begin_memo ())
             ; l_memo_iter != l_expt_iter->end_memo ()
             ; ++l_memo_iter)
        {
          // Search a_compound_map for the compound/formula.
          const key_type l_key (l_memo_iter->compound_label (), l_memo_iter->formula_label ());
          compound_map_type::iterator l_cursor (a_compound_map.find (l_key));
          if (l_cursor != a_compound_map.end ())
          {
            if (NULL == l_cursor->second) // A compound_ref reference, replace nul
            {
              l_cursor->second = &(*l_memo_iter);
            }
            else
            {
              a_compound_map.insert(l_cursor, compound_map_type::value_type (l_key, &(*l_memo_iter)));
            }
          }
        }
      }
    }
  }

}

/**
 * Generate the proj/comp/form/memo report of a_project on a_os.
 */
void project_cfm_report::operator()(const data_scheme::project & a_project, std::ostream & a_os) const 
{
  compound_map_type l_compound_map (&compare_pair);
  {
    data_scheme::scoped_open< data_scheme::project > l_local_open (a_project);
    generate_map (a_project, l_compound_map);
  }
  write_map (a_os, l_compound_map, a_project.label ());

}

/**
 * Write the report.
 */
void project_cfm_report::write_map(std::ostream & a_os, const project_cfm_report::compound_map_type & a_compound_map, const std::string& a_project_label) const 
{
  const std::string l_compound_name (data_access::names::compound_name ());
  const std::string l_formula_name (data_access::names::formula_name ());
  const std::string l_label_name (data_access::names::label_name ());
  // write project proxy
  data_access::xml_writer_proxy l_project (a_os
      , data_access::names::project_name ()
      , data_access::xml_access_helper::helper ().public_id ()
      , data_access::xml_access_helper::helper ().system_id ());
  l_project.attribute (l_label_name, a_project_label);
  
  { // Scope for l_compound_proxy and l_formula_proxy
    std::auto_ptr< data_access::xml_writer_proxy > l_compound_proxy, l_formula_proxy;
    std::string l_current_compound_label, l_current_formula_label;
    for (compound_map_type::const_iterator l_map_entry (a_compound_map.begin ())
         ; l_map_entry != a_compound_map.end ()
         ; ++l_map_entry)
    {
      COMPCHEM_CHECK (not l_map_entry->first.first.empty (), "Empty compound label!");
      COMPCHEM_CHECK (not l_map_entry->first.second.empty (), "Empty formula label!");
      if (NULL == l_compound_proxy.get ()
          or l_current_compound_label != l_map_entry->first.first) // changed compound
      {
        l_current_compound_label = l_map_entry->first.first;
        l_compound_proxy.reset (new data_access::xml_writer_proxy (l_project, l_compound_name));
        l_compound_proxy->attribute (l_label_name, l_current_compound_label);
        l_formula_proxy.reset ();
        l_current_formula_label.clear ();
      }
      if (NULL == l_formula_proxy.get ()
          or l_current_formula_label != l_map_entry->first.second) // changed formula
      {
        l_current_formula_label = l_map_entry->first.second;
        l_formula_proxy.reset (new data_access::xml_writer_proxy (*(l_compound_proxy.get ()), l_formula_name));
        l_formula_proxy->attribute (l_label_name, l_current_formula_label);
      }
      data_access::xml_writer_proxy l_memo_proxy (*(l_formula_proxy.get ()), data_access::names::memo_src_name ());
      COMPCHEM_CHECK (not l_map_entry->second->label ().empty (), "Empty memo label!");
      l_memo_proxy.attribute (l_label_name, l_map_entry->second->label ());
      l_memo_proxy.attribute (l_compound_name, l_current_compound_label);
      l_memo_proxy.attribute (l_formula_name, l_current_formula_label);
      // Construct href
      std::string l_href;
      if (not l_map_entry->second->is_src ())
      {
        std::stringstream l_ss;
        // Need to make up the href as
        // %database_uri%/program/project[@label='%proj%']/experiment[@reference='%ref%']/memo[@label='%label%']
        // or
        // %project_src_uri%/project[@label='%proj%']/experiment[@reference='%ref%']/memo[@label='%label%'
        l_ss << l_map_entry->second->page ().section ().uri_scheme () << "://" << l_map_entry->second->page ().section ().uri_path ();
        if (not l_map_entry->second->page ().section ().is_src ()) // need to append project info
        {
          l_ss << "/" << data_access::names::project_name () << "[@" << l_label_name << "='" << l_map_entry->second->page ().section ().label () << "']";
        }
        // Now add experiment and memo.
        l_ss << "/" << data_access::names::experiment_name () << "[@" << data_access::names::reference_name () << "='" << l_map_entry->second->page ().reference () << "']/" << data_access::names::memo_name () << "[@" << l_label_name << "='" << l_map_entry->second->label () << "'";
        l_href.assign (l_ss.str ());
      }
      else
      {
        l_href = l_map_entry->second->uri ();
      }
      COMPCHEM_CHECK (not l_href.empty (), "Empty memo href!");
      l_memo_proxy.attribute (data_access::names::href_name (), l_href);
    }
  }

}


} // namespace report
