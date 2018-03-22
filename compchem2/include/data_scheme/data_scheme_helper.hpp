#ifndef COMPCHEM_DATA_SCHEME_DATA_SCHEME_HELPER_HPP
#define COMPCHEM_DATA_SCHEME_DATA_SCHEME_HELPER_HPP

// ------------------------------------------------------------
// class: data_scheme_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>

namespace utility { class program_options; } 

namespace data_scheme {

/**
 * This singleton class manages configuration information for the data_scheme library.
 * 
 * The target_project is the project of current interest. The library is free to not load
 * project files that do not match the target.
 * 
 * Added options:
 *   project,p = The target project for this application run.
 *   auto-open-memo,A = read all the memo-src objects automatically.
 *   auto-project,P = attempt to create a new project if not present
 */
class data_scheme_helper
{
  private:
    /**
     * Should we automatically open memo objects?
     */
    bool m_auto_memo;

    /**
     * Whether to automatically create missing target projects.
     */
    bool m_auto_project;

    std::string m_target_project;

    /**
     * The Singleton object.
     */
    static data_scheme_helper s_exemplar;


  public:
    /**
     * When this is true, the user indicated that they wanted the
     * application to attempt to automatically open the target of
     * memo-src objects while reading the parent document.
     */
    bool auto_open_memo() const
    {
      return m_auto_memo;
    }

    /**
     * When this is true, the user indicated that they wanted the
     * application to attempt to automatically open the target of
     * memo-src objects while reading the parent document.
     */
    void auto_open_memo(bool a_automake)
    {
      m_auto_memo = a_automake;
    }
    

    /**
     * When this is true, the user indicated that they wanted the
     * application to attempt to automatically create a new project
     * when it was not already present.
     */
    bool auto_project() const
    {
      return m_auto_project;
    }

    /**
     * When this is true, the user indicated that they wanted the
     * application to attempt to automatically create a new project
     * when it was not already present.
     */
    void auto_project(bool a_automake)
    {
      m_auto_project = a_automake;
    }
    


  private:
    /**
     * Constructor for singleton.
     */
    data_scheme_helper();


  public:
    ~data_scheme_helper() {}


  private:
    /**
     * no copy
     */
    data_scheme_helper(const data_scheme_helper & source);


  public:
    /**
     * Get the singleton.
     */
    static data_scheme_helper & helper()
    {
      return s_exemplar;
    }


  private:
    /**
     * no assign
     */
    data_scheme_helper & operator=(const data_scheme_helper & source);

    /**
     * Add options to the configuration helper.
     */
    void register_options();


  public:
    /**
     * The label of the target project, default is "default"!
     */
    const std::string & target_project() const
    {
      return m_target_project;
    }

    /**
     * Set the target project from a std string.
     */
    void target_project(const std::string& a_label)
    {
      m_target_project = a_label;
    }


};

} // namespace data_scheme
#endif
