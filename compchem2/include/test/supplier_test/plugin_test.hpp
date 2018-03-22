#ifndef COMPCHEM_SUPPLIER_PLUGIN_TEST_HPP
#define COMPCHEM_SUPPLIER_PLUGIN_TEST_HPP

// ------------------------------------------------------------
// class: plugin_test
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "supplier/basic_filter_traits.hpp"
#include <string>
#include <memory>
#include <boost/bind.hpp>


// Manual includes
#include <vector>
// -
namespace supplier { class filter_manager; } 

namespace supplier {

/**
 * Test the plugin system.  This class should exercise the whole 
 * filter system, in particular allowing plugin filters to 
 * be tested individually.
 * 
 * When compiled as a program it defines the following options:
 * -f|--filename [name]  =  filename of gaussian log file to process
 * -p|--plugin [name] = filename of plugin to test (may be given multiple times)
 * -?|-h|--help = help message
 * 
 * TODO : install error  handler.
 */
class plugin_test {
  private:
    std::string m_conf_file;

    /**
     * Flag an error occured
     */
    bool m_error;

    /**
     * The name of a gaussian log file
     */
    std::vector< std::string > m_filenames;

    /**
     * The plugin manager instance.
     */
    std::auto_ptr<filter_manager> m_manager;

    /**
     * The 'main' return value.
     */
    int m_result;

    /**
     * Call back to add to m_manager to get notification when an error occurs.
     */
    void error_method(std::string a_msg, int a_id);

    /**
     * Run the application.  This should be run after setting up
     * the m_conf_file and m_filenames.
     * 
     * \pre (not m_conf_file.empty and exists(m_conf_file)) OR nul /= m_manager
     * \pre not m_filenames.empty and for_each(m_filenames, exist)
     */
    void main();


  public:
    /**
     * Return a pretty name for this object
     */
    std::string title() { return "Plugin Test"; }

    /**
     * Has there been an error?
     */
    bool on_error() const { return m_error; }


  private:
    /**
     * no copy
     */
    plugin_test & operator=(const plugin_test & source);


  public:
    /**
     * Default Constructor
     */
    plugin_test(int argc, char ** argv);

    ~plugin_test() {}


  private:
    /**
     * no copy
     */
    plugin_test(const plugin_test & source);

    /**
     * Process the command line options.  Returns true if options
     * where processed successfully.
     */
    bool process_options(int argc, char ** argv);


  public:
    /**
     * Get th 'main' return value
     */
    int result() const { return m_result; }

};

} // namespace supplier
#endif
