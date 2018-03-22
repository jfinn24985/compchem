#ifndef COMPCHEM_SUPPLIER_FILTER_MANAGER_HPP
#define COMPCHEM_SUPPLIER_FILTER_MANAGER_HPP

// ------------------------------------------------------------
// class: filter_manager
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/basic_filter.hpp"
#include "data_access/names.hpp"
#include "data_scheme/program.hpp"
#include <boost/ptr_container/ptr_map.hpp>

#include "supplier/supplier_constants.hpp"
#include <string>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


#include <set>
#include "supplier/supplier_error.hpp"
#include <ostream>


// Manual includes
#include <vector>
#include <algorithm>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/function.hpp>
// --
namespace data_scheme { class content_visitor; } 
namespace data_scheme { class datum; } 
namespace data_scheme { class experiment; } 
namespace utility { class filesystem; } 
namespace supplier { class matcher_filter; } 
namespace data_scheme { class mechanism; } 
namespace data_access { class persistence; } 
namespace supplier { class plugin_filter; } 
namespace data_scheme { template<class t_src_type> class scoped_open; } 
namespace supplier { class supplier_helper; } 
namespace data_scheme { class filter; } 
namespace data_scheme { class memo; } 
namespace data_scheme { class project; } 

namespace supplier {

/**
 * This manages the processing of plugin-filters over a raw data file.
 * 
 * On reprocessing a memo, a non-empty memo with the mechanism set
 * will be assumed to have the correct mechanism. In this case the datum
 * with outcome containing the word 'detect' will be used to intialise the
 * filters. When not present no string will be used to intialise the filters.
 * 
 * ROLE:
 * * Given a configuration file, this collects a set of appropriate filters.
 * (These filters are made available for dynamic configuration (including removal), 
 * and mechanism/subtype can be checked)
 * * Filters are then applied to the raw data file and the
 * extracted data is added to a DOM twiglet for entry into the data system.
 * 
 * Plugin description file looks something like:
 * <group [label="??"]> <!-- label here is informative only -->
 *  <filetype ....> <!-- see supplier_file_manager -->
 *  <mechanism label="??" />(*) <!-- label to match with filetype -->
 *  <plugin src="??" !public="??"! !options=?? name=??! />(*)  <!-- locations to retrieve plugin from.
 * In future a public label may be used for XML catalog style plugin lookup.
 * In future options/name will be available for dynamic filter configuration. -->
 * </group>
 */
class filter_manager {
  private:
    /**
     * The iterator type for the filter objects.
     */
    typedef boost::ptr_vector< basic_filter >::iterator iterator;

    /**
     * The const iterator type for the filter objects.
     */
    typedef  boost::ptr_vector< basic_filter >::const_iterator const_iterator;


  public:
    typedef boost::ptr_map< std::string,
         boost::ptr_vector< basic_filter > > map_t;


  private:
    /**
     * The map/ list of currently active filters.
     */
    boost::ptr_map< std::string,
     boost::ptr_vector<basic_filter> > m_filters; 

  public:
    /**
     * The number of filters already created for this (mechanism) label.
     * 
     * \result 0 if no filter_set[label] or filter_set[label].empty otherwise filter_set[label].size
     */
    unsigned int count_filters(const std::string& a_label)
    {
      unsigned int Result (0);
      if (m_filters.count (a_label) != 0) Result = m_filters[a_label].size ();
      return Result;
    }

    /**
     * Instantiate the filters contained in a_mechanism, ignoring 'detect' filters.
     * 
     * \pre not a_mechanism.empty
     * \post 0 != m_filter.count (a_mechanism)
     */
    void discover_filters(const data_scheme::mechanism & a_mechanism);

    /**
     * Instantiate the 'detect' filters for all mechanisms of a_prog.
     * 
     * \post 0 != m_filter.count ('detect')
     */
    void discover_filters(data_scheme::program::const_mechanism_iterator a_begin, data_scheme::program::const_mechanism_iterator a_end);


  private:
    /**
     * Construct a basic_filter object from the a_filter XML element.
     * 
     * * Detect filter type [regex:plugin(:python)] and instantiate relevant
     * filter type.
     */
    static std::auto_ptr< basic_filter > make_filter(const data_scheme::filter & a_filter);


  public:
    /**
     * The main operation function to update a_memo with a possibly new result 
     * file.
     * 
     * Internally this:
     * 1: determines the mechanism of a_path and sets it on a_memo
     * 2: loads the filters for the mechanism.
     * 3: creates a working filter set by removing filters that have already been applied
     *     to a_memo or do not supply outcomes in the project.
     * 4: calls apply_filters
     * 
     * \pre a_memo.href (implies a_memo.target_mechanism) or exists (a_path)
     * \post a_memo.href implies a_memo.target_mechanism
     */
    void process(data_scheme::memo & a_memo, const boost::filesystem::path & a_path);

    /**
     * This method update the content of a_project with the project's current 
     * outcome  set. This scans through the memo objects and updates their 
     * results. For example this could be done after a project has added a new 
     * outcome label.
     * 
     * Internally for each memo in a_project
     * 1a: if memo.href but not memo.target_mechanism then call process (memo, "")
     * 1b: else loads the filters for memo.target_mechanism.
     *   2: creates a working filter set by removing filters that have already been applied
     *     to memo or do not supply outcomes in the project.
     *   3: calls apply_filters
     */
    void process(data_scheme::project & a_project);


  private:
    /**
     * This method applies filters to the file named in a_twiglet.href then
     * adds the results to a_twiglet. It limits the range a_filter_begin to
     * a_filter_end to those with outcomes contained in a_out_begin to
     * a_out_end.
     * This method operates in two modes.  When a_find_first is true it 
     * is being called to detect the mechanism of a_twiglet.href.  In this 
     * mode it stops processing when the first filter object returns 
     * COMPLETE. The results from the filter are added to a_twiglet as
     * usual, however the a_twiglet.target_mechanism is also set. When 
     * a_find_first is false then it will process the entire file and filters and
     * add all the results to a_twiglet.
     */
    int apply_filters(data_scheme::memo & a_twiglet, std::set< std::string >::const_iterator a_out_begin, std::set< std::string >::const_iterator a_out_end, iterator a_filter_begin, iterator a_filter_end, bool a_find_first) throw(supplier_error);


  public:
    /**
     * Construct a filter manager that uses a configuration file for finding 
     * filetypes and filters.
     * 
     * \param a_conf_filename - The name of the configuration file to use.
     */
    filter_manager();

    /**
     * Destructor, does nothing special.
     */
    ~filter_manager()
    {}


  private:
    /**
     * non-copyable
     */
    filter_manager(const filter_manager & source);

    /**
     * non-copyable
     */
    filter_manager & operator=(const filter_manager & source);


  public:
    /**
     * Print information from the list of currently loaded filters.  This is primarily a 
     * logging/debugging aid.
     */
    template< typename charT, typename traits > inline void serialise(std::basic_ostream<charT, traits> & a_os) const;


};
/**
 * Print information from the list of currently loaded filters.  This is primarily a 
 * logging/debugging aid.
 */
template< typename charT, typename traits > inline void filter_manager::serialise(std::basic_ostream<charT, traits> & a_os) const 
{
  for (map_t::const_iterator l_cursor (m_filters.begin ());
      l_cursor != m_filters.end ();
      ++l_cursor)
  {
    for (const_iterator l_i (l_cursor->begin ()); l_i != l_cursor->end (); ++l_i)
    {
      a_os << (*l_i);
    }
  }

}

// Friendly serialise method
template < typename charT, typename traits > static inline
  std::ostream &
operator<< (std::basic_ostream < charT, traits > &a_os, const filter_manager & a_man)
{
  a_man.serialise (a_os);
  return a_os;
}

} // namespace supplier
#endif
