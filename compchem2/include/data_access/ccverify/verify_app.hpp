#ifndef VERIFY_APP_HPP
#define VERIFY_APP_HPP

// ------------------------------------------------------------
// class: verify_app
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <vector>
#include <string>

namespace utility { class program_options; } 
namespace data_scheme { class program; } 
namespace data_access { class verify_visitor; } 
namespace data_access { class xml_access_helper; } 
namespace data_scheme { class data_scheme_helper; } 

namespace data_access {

/**
 * Check and repair a cchem database.
 * 
 * Some parts of the repair process are already provided by the 
 * data_access library. This class facilitates those operations 
 * as well as extending the verifcation/repair process.
 * 
 * * DTD validity.
 *  <= provided by XML reader (extended functionality)
 * * Broken project-src and memo-src links.
 *  <= provided by data_access library
 * 
 * 
 * * Verify that all reference attributes have a target object:
 *  <= internal
 *  * compound-ref -> compound/formula
 *  * experiment -> worker
 *  * memo -> mechanism/filter, compound/formula
 *  * project -> worker
 *  * settings -> (parent)/filter
 */
class verify_app {
  public:
    typedef std::vector< std::string >::const_iterator const_iterator;


  private:
    /**
     * The set of filenames to process
     */
    std::vector< std::string > m_fileset;


  public:
    /**
     * Get a 'begin' iterator to the list of files to process.
     */
    const_iterator begin()
    {
      return m_fileset.begin ();
    }

    verify_app()
    : m_fileset ()
    {}

    ~verify_app() {}


  private:
    /**
     * no copy
     */
    verify_app(const verify_app & source);


  public:
    /**
     * Are there any files to process.
     */
    bool empty() { return m_fileset.empty (); }

    /**
     * Get an 'end' iterator to the list of files to process.
     */
    const_iterator end() { return m_fileset.end (); }

    /**
     * Perform the main operations of the crawler.
     */
    int main(int argc, char ** argv);


  private:
    /**
     * no copy 
     */
    verify_app & operator=(const verify_app & source);

};

} // namespace data_access
#endif
