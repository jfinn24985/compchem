#ifndef COMPCHEM_CCFILTER_APP_HPP
#define COMPCHEM_CCFILTER_APP_HPP

// ------------------------------------------------------------
// class: ccfilter_app
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "programs/basic_application.hpp"
#include <boost/lexical_cast.hpp>

#include <boost/bind.hpp>

#include <boost/program_options/options_description.hpp>

#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>



namespace data_scheme { class compound; } 
namespace data_scheme { class data_scheme_helper; } 
namespace data_scheme { class experiment; } 
namespace supplier { class filter_manager; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class program; } 
namespace utility { class program_options; } 
namespace data_scheme { class project; } 
namespace supplier { class supplier_helper; } 
namespace utility { class time_stamp; } 

namespace ccfilter {

/**
 * This program filters a raw file into a data base.  No "locking" of the database file is performed.  It can be used from the command line.  It is also designed to be usable as the back-end for a GUI application.
 * 
 * Program options are:
 * 
 * (data_access_helper)
 *     autofix,A = automatically rebuild broken cross-references in database
 *     search-dir,D = directory paths to search for broken cross-references
 *     ** hidden options **
 *     public-id = the public ID to use for new XML documents.
 *     system-id = the system ID to use for new XML documents.
 * (data_scheme_helper)
 *     project,p = The target project for this application run.
 *     auto-open-memo,M = read all the memo-src objects automatically.
 *     auto-project,P = attempt to create a new project if not present
 * (supplier helper)
 *     experiment,e = The target experiment for this application run.
 *     file,i = The target result file for this application run.
 *     memo,m = The target memo for this application run.
 *     memo-src,s = The location for memo-src for this application run
 *     filter-dir,F = directory paths to search for filters (may be used multiple times).
 *     compound,c = The target compound for the target memo (req for new memos)
 *     formula,f = The target formula for the target memo (req for new memos)
 * 
 * (ccfilter)
 *     ((interactive = Will ask user for missing data instead of failing))
 *     database <uri> = The database file, default location is ~/.chkcalc/db/main-database.xml
 */
class ccfilter_app : public programs::basic_application<ccfilter_app> {
  private:
    /**
     * The database URI.
     */
    std::string m_database;


  public:
    ccfilter_app()
    : m_database ()
    {}

    ~ccfilter_app() {}


  private:
    ccfilter_app(const ccfilter_app & source);

    ccfilter_app & operator=(const ccfilter_app & source);


  public:
    /**
     * Process the input file.
     */
    int main();

    /**
     * Create a "unique" filename for the memo-src location.
     */
    std::string memo_src_location(const boost::filesystem::path & a_path, const std::string& a_memo_label);

    /**
     * Register options specific to the ccfilter class.
     */
    void register_options();

};

} // namespace ccfilter
#endif
