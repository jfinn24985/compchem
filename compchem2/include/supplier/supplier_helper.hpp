#ifndef COMPCHEM_SUPPLIER_HELPER_HPP
#define COMPCHEM_SUPPLIER_HELPER_HPP

// ------------------------------------------------------------
// class: supplier_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <vector>
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>



namespace utility { class program_options; } 

namespace supplier {

/**
 * This singleton class manages configuration information for the supplier library.
 * 
 * Registered options
 *   experiment,e = The target experiment for this application run.
 *   file,i = The target result file for this application run.
 *   memo,m = The target memo for this application run.
 *   filter-dir,F = directory paths to search for filters (may be used multiple times).
 *   compound,c = The target compound for the target memo (req for new memos)
 *   formula,f = The target formula for the target memo (req for new memos)
 */
class supplier_helper
{
  public:
    typedef std::vector< std::string >::const_iterator dir_iterator;


  private:
    /**
     * The set of directories to search for filters.
     */
    std::vector<std::string> m_plugin_dirs;

    /**
     * The target_memo's compound reference.
     */
    std::string m_target_compound;

    /**
     * The target experiment.
     */
    std::string m_target_experiment;

    /**
     * The target file for the filter.
     */
    boost::filesystem::path m_target_file;

    /**
     * The target_memo's compound's formula.
     */
    std::string m_target_formula;

    /**
     * The target memo for the filter/parser.
     */
    std::string m_target_memo;

    /**
     * A path for the memo-src file.
     */
    std::string m_target_memo_src;

    /**
     * The Singleton object.
     */
    static supplier_helper s_exemplar;


  public:
    /**
     * Add a single directory name to the list of directories.
     */
    void add_directory(const std::string& a_dirname)
    {
      m_plugin_dirs.push_back (a_dirname);
    }

    dir_iterator begin_directory() const
    {
      return m_plugin_dirs.begin ();
    }

    dir_iterator end_directory() const
    {
      return m_plugin_dirs.end ();
    }


  private:
    /**
     * Constructor for singleton.
     */
    supplier_helper();


  public:
    ~supplier_helper() {}


  private:
    /**
     * no copy
     */
    supplier_helper(const supplier_helper & source);


  public:
    /**
     * Get the singleton.
     */
    static supplier_helper & helper()
    {
      return s_exemplar;
    }


  private:
    /**
     * no assign
     */
    supplier_helper & operator=(const supplier_helper & source);

    /**
     * Add options to the configuration helper.
     */
    void register_options();


  public:
    /**
     * The label of the target project, default is "default"!
     */
    const std::string & target_compound() const
    {
      return m_target_compound;
    }

    /**
     * Set the target project from a std string.
     */
    void target_compound(const std::string& a_label)
    {
      m_target_compound.assign (a_label);
    }

    /**
     * The label of the target project, default is "default"!
     */
    const std::string & target_experiment() const
    {
      return m_target_experiment;
    }

    /**
     * Set the target project from a std string.
     */
    void target_experiment(const std::string& a_label)
    {
      m_target_experiment.assign (a_label);
    }

    /**
     * The label of the target project, default is "default"!
     */
    const boost::filesystem::path & target_file() const
    {
      return m_target_file;
    }

    /**
     * Set the target project from a std string.
     */
    void target_file(const std::string& a_label)
    {
      m_target_file = a_label;
    }

    /**
     * The label of the target project, default is "default"!
     */
    const std::string & target_formula() const
    {
      return m_target_formula;
    }

    /**
     * Set the target project from a std string.
     */
    void target_formula(const std::string& a_label)
    {
      m_target_formula.assign (a_label);
    }

    /**
     * The label of the target project, default is "default"!
     */
    const std::string & target_memo() const
    {
      return m_target_memo;
    }

    /**
     * Set the target project from a std string.
     */
    void target_memo(const std::string& a_label)
    {
      m_target_memo.assign (a_label);
    }

    /**
     * The label of the target project, default is "default"!
     */
    const std::string & target_memo_src() const
    {
      return m_target_memo_src;
    }

    /**
     * Set the target project from a std string.
     */
    void target_memo_src(const std::string& a_label)
    {
      m_target_memo_src.assign (a_label);
    }


};

} // namespace supplier
#endif
