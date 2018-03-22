#ifndef COMPCHEM_NEWCALC_NEWCALC_HPP
#define COMPCHEM_NEWCALC_NEWCALC_HPP

// ------------------------------------------------------------
// class: newcalc
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include <new>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>




namespace cchem { namespace newcalc { class scope; }  } 

namespace cchem {

namespace newcalc {

/**
 * Newcalc is a script writer; it manages the construction of a series of scripts.
 */
class newcalc {
  public:
    typedef guistate guistate_type;


  private:
    /**
     * The owning guistate object.  This is not set until register_with_x11 is
     * called.
     */
    guistate_type * m_parent;

    /**
     * The application object.
     */
    static newcalc * m_application;

    /**
     * The suppposed class name (blank here)
     */
    static const std::string m_class;

    /**
     * The application name (derived from the name the program was called as.
     */
    static const std::string m_name;


  public:
    /**
     * The X11 application name.  This can simply be a value derived from argv[0].
     */
    static const std::string & application_name() throw() {
      return m_name; }

    /**
     * The X11 application class name. If this an empty string it will be derived from the application name
     */
    static const std::string& application_class() {
      return m_class; }


  private:
    /**
     * Close the application
     */
    void close() throw() {
      COMPCHEM_REQUIRE (NULL != m_parent, "Can not call close without guistate parent");
      m_parent->close (); }

    /**
     * Write the script file.
     */
    void data_write() throw(std::bad_alloc);


  public:
    /**
     * This method is called after the user interface has been created but before it is displayed. It
     * is the place for the application to push data into the interface.
     */
    void display(guistate_type * self) throw() {}


  private:
    /**
     * Find occurences of '%%var%%' and return "var" and set a_start and a_end
     */
    std::string find_variable(const std::string& a_str, unsigned int & a_start, unsigned int & a_end) throw(std::bad_alloc);

    /**
     * This method creates the result files. We create the result directory and 
     * filenames on-the-fly. 
     */
    void generate(scope & a_scope, const boost::filesystem::path & a_dir) throw(std::bad_alloc);

    /**
     * Convert the input string into final form by making substitutions from a_scope
     * and elsewhere.
     */
    std::string make_substitutions(const std::string& a_str, const scope & a_scope) throw(std::bad_alloc);


  public:
    newcalc(int & a_argc, char ** a_argv) throw();

    ~newcalc() throw() {}


  private:
    /**
     * no copy
     */
    newcalc(const newcalc & source);


  public:
    /**
     * This method is registered with the Xmt system and acts as trigger callback.
     */
    static void notifier(widget_t a_widg, void * client_data, void * calldata) throw();


  private:
    /**
     * no assign
     */
    newcalc & operator=(const newcalc & source);


  public:
    /**
     * This method is called before attempting to create an X11 connection. It should be
     * used to register static information with the library.  For example this is the 
     * place to register functions and widget types with the Xmt resource converter.
     */
    void register_with_x11(guistate_type * a_self) throw();

    /**
     * The X11 application resorce filename. This may not exist, the guistate system
     * will check for this file, then attempt to look for other files in standard locations.
     */
    static const std::string& resource_file() throw() {
      const static std::string Result("/home/finnerty/lib/app-defaults/NewCalc.ad");
      // const static std::string Result("NewCalc.ad");
      return Result; }


  private:
    /**
     * Write a subsection starting from a_base resource path onto a_os stream.  Substitutions should be made from a_scope.
     */
    void write_body(const resource_path & a_base, scope & a_scope, std::ostream & a_os) throw(std::bad_alloc);

};

} // namespace cchem::newcalc

} // namespace cchem
#endif
