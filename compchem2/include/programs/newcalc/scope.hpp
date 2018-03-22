#ifndef COMPCHEM_NEWCALC_SCOPE_HPP
#define COMPCHEM_NEWCALC_SCOPE_HPP

// ------------------------------------------------------------
// class: scope
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


// Manual includes
#include <map>
// -
namespace cchem {

namespace newcalc {

/**
 * A scope is the table of entities that are available to construct the script.  It acts as the interface
 * between the datasoure and the interpreter.
 * 
 * ROLE: Objects of this type mediate the retrieval of information from the resource database.
 * 
 * SCOPE: This base object provides an interface where the only indication of the underlying
 * database is the resource_path and string based keys.
 */
class scope {
  public:
    /**
     * Get the template's base resource path.
     */
    virtual const resource_path & base() const throw() = 0;

    /**
     * A test version of value() where a_result contains a meaningful value if
     * the return value is true.  This should be used when you want to retrieve
     * a value only if it is truly present.
     */
    virtual bool has_value(const resource_path & a_path, std::string & a_result) const throw(std::bad_alloc) {
    bool Result (this->has_value_virt (a_path, a_result));
    if (m_log) {
      if (Result) {
        std::cout << "## Got configuration [" << a_path.string () << "], value [" << a_result << "]\n"; }
      else {
        std::cout << "## No configuration for [" << a_path.string () << "]\n"; }}
    return Result; }

    /**
     * A test version of value() where a_result contains a meaningful value if
     * the return value is true.  This should be used when you want to retrieve
     * a value only if it is truly present.
     */
    virtual bool has_value_virt(const resource_path & a_path, std::string & a_result) const throw(std::bad_alloc) = 0;


  private:
    /**
     * no assign
     */
    scope & operator=(const scope & source);


  public:
    scope() throw() {};


  private:
    /**
     * no copy
     */
    scope(const scope & source);


  public:
    virtual ~scope() throw() {}

    /**
     * Return the value of variable a_key in the given resource path context
     */
    std::string value(const std::string& a_key, const resource_path & a_path) const throw(std::bad_alloc) {
      return value (a_path / a_key); }

    /**
     * Return the value of variable a_key in the given resource path context
     */
    std::string value(const resource_path & a_path) const throw(std::bad_alloc) {
      std::string Result;
      has_value (a_path, Result); 
      return Result; }

  private:
    /**
     * Whether to produce log output of the configuration searches.
     */
    static bool m_log;

};
/**
 * A decoration for the scope class that adds the ability to fix some of the 'attribute' values.
 */
class scope_local : public scope {
  public:
    typedef std::map< std::string, std::string >::const_iterator const_iterator;


  private:
    scope & m_up;

    /**
     * The local value map
     */
    mutable std::map< std::string, std::string > m_local;


  public:
    /**
     * Add a local value. It may replace an existing value.
     */
    void add_local(const std::string& a_key, const std::string& a_val) throw(std::bad_alloc) {
      m_local [a_key] = a_val; }

    /**
     * Get the template's base resource path.
     */
    virtual const resource_path & base() const throw() {
      return m_up.base (); };


  protected:
    /**
     * A test version of value() where a_result contains a meaningful value if
     * the return value is true.  This should be used when you want to retrieve
     * a value only if it is truly present.
     */
    bool has_value_virt(const resource_path & a_path, std::string & a_result) const throw(std::bad_alloc);

  public:
    explicit scope_local(scope & a_up) throw(std::bad_alloc)
    : scope(), m_up (a_up), m_local () {}
    

    virtual ~scope_local() throw() {}

};
/**
 * An object of this class mediates with the main database.
 */
class scope_working : public scope {
  public:
    typedef guistate guistate_type;


  private:
    /**
     * The base resource path of this scope
     */
    const resource_path m_path;

    /**
     * The parent context from which to obtain values.
     */
    guistate_type & m_parent;


  public:
    /**
     * Get the template's base resource path.
     */
    const resource_path & base() const throw() {
      return m_path; }


  protected:
    /**
     * A test version of value() where a_result contains a meaningful value if
     * the return value is true.  This should be used when you want to retrieve
     * a value only if it is truly present.
     */
    virtual bool has_value_virt(const resource_path & a_path, std::string & a_result) const throw(std::bad_alloc) {
      return parent ().get_configuration_string (a_path.string (), a_result); }


  public:
    guistate_type & parent() const throw() {
      return m_parent; };

    scope_working(const resource_path & a_base, guistate_type & a_parent) throw(std::bad_alloc)
    : m_path (a_base)
    , m_parent (a_parent) {}

    virtual ~scope_working() throw() {}

};

} // namespace cchem::newcalc

} // namespace cchem
#endif
