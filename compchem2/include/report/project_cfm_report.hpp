#ifndef COMPCHEM_PROJECT_CFM_REPORT_HPP
#define COMPCHEM_PROJECT_CFM_REPORT_HPP

// ------------------------------------------------------------
// class: project_cfm_report
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_access/names.hpp"
#include <string>

// Manual includes
#include <map>
// -
namespace data_scheme { class compound; } 
namespace data_scheme { class compound_ref; } 
namespace data_scheme { class experiment; } 
namespace data_scheme { class formula; } 
namespace data_scheme { class program; } 
namespace data_scheme { class project; } 
namespace data_scheme { template<class t_src_type> class scoped_open; } 
namespace data_scheme { class memo; } 
namespace data_access { class xml_access_helper; } 
namespace data_access { class xml_writer_proxy; } 

namespace report {

/**
 * The class generates a compound-formula-memo document
 * based on a given project.
 * 
 * It generates a report with the following XML psuedo-structure:
 * 
 * project/compound/formula/memo-src
 * 
 * Which includes all memo objects within the project plus any
 * memos in the program that match project/compound_ref 
 * + project/@outcomes.
 */
class project_cfm_report
{
  public:
    typedef std::pair< std::string, std::string > key_type;

    /**
     * Map of [compound_label] --> (is_from_ref, fomula_map)
     * 
     * Only needs to be multi_map [(compd_lbl, formula_lbl)] --> memo
     */
    typedef std::multimap< std::pair< std::string, std::string >, const data_scheme::memo *, bool (*)(const std::pair<std::string, std::string >&, const std::pair<std::string, std::string >&) > compound_map_type;


  private:
    /**
     * Add the referenced compound/formula(e). This inserts an entry
     * into a_compound_map with a nul for the memo ptr (iff no entry
     * already exists).
     */
    void add_reference(const data_scheme::compound_ref & a_ref, compound_map_type & a_compound_map) const;

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
    void generate_map(const data_scheme::project & a_parent, compound_map_type & a_compound_map) const;


  public:
    /**
     * Generate the proj/comp/form/memo report of a_project on a_os.
     */
    void operator()(const data_scheme::project & a_project, std::ostream & a_os) const;


  private:
    /**
     * no assign
     */
    project_cfm_report & operator=(const project_cfm_report & source)
    {
      return *this;
    }

  public:
    /**
     * Create the report.
     */
    project_cfm_report() throw ()
    {}

    ~project_cfm_report() throw ()
    {}

    project_cfm_report(const project_cfm_report & source)
    {}


  private:
    /**
     * Write the report.
     */
    void write_map(std::ostream & a_os, const compound_map_type & a_compound_map, const std::string& a_project_label) const;


};

} // namespace report
#endif
