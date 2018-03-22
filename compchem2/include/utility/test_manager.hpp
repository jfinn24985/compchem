#ifndef COMPCHEM_TEST_MANAGER_HPP
#define COMPCHEM_TEST_MANAGER_HPP

// ------------------------------------------------------------
// class: test_manager
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <vector>
#include <boost/function.hpp>

#include <string>
#include <memory>

namespace utility { class program_options; } 

namespace utility {

/**
 * Base class for types that want to utilise the unit-test framework.  Most test classes
 * should inherit this class indirectly through the unit_test class.
 */
class base_test {
  public:
    typedef boost::function<void (void)> funtion_t;

    /**
     * prefix for commentary messages
     */
    static const std::string COMM;

    /**
     * prefix for exception messages
     */
    static const std::string EXCP;

    /**
     * prefix for messages with info to check or compare manually
     */
    static const std::string CHCK;

    /**
     * prefix for messages with info to watch
     */
    static const std::string WTCH;

    /**
     * prefix for section titles
     */
    static const std::string TTLE;

    /**
     * prefix for messages in program branches when the desired branch is reached
     */
    static const std::string IFYS;

    /**
     * prefix for messages in program branches when the undesired branch is reached
     */
    static const std::string IFNO;

    virtual ~base_test() {}

    /**
     * Get the reference to the logging stream.
     */
    static std::ostream & log() throw();

    /**
     * If a_test is true print a_msg. If false throws logic_error with a_failmsg.
     * 
     * \pre a_msg /= nul and a_failmsg /= nul
     */
    static inline void bool_test(bool a_test, std::string a_msg);

    /**
     * If a_test is true print a_msg. If false throws logic_error with a_failmsg.
     * 
     * \pre a_msg /= nul and a_failmsg /= nul
     */
    static inline void bool_test(bool a_test, const char *const a_msg);

    /**
     * This method calls a_fn and expects to catch a contract_error of a_level.
     * 
     * Throws logic_errors if function fails to throw an exception and rethrows
     * the contract_error if of the wrong type.
     */
    static void monitor_function(unsigned int a_lvl, funtion_t a_fn, const std::string& a_msg=std::string())
    {
      if (a_msg.empty ())
      {
        const char* l_tmp (NULL);
        monitor_function (a_lvl, a_fn, l_tmp);
      }
      else
      {
        monitor_function (a_lvl, a_fn, a_msg.c_str ());
      }
    }

    /**
     * This method calls a_fn and expects to catch a contract_error of a_level.
     * 
     * Throws logic_errors if function fails to throw an exception and rethrows
     * the contract_error if of the wrong type.
     */
    static void monitor_function(unsigned int a_lvl, funtion_t a_fn, const char *const a_msg);

    /**
     * Override in derived classes to perform unit test.
     * 
     * Failed tests are indicated by an uncaught exception.
     */
    virtual void main_test() const = 0;

    virtual std::string title() const = 0;

};
/**
 * If a_test is true print a_msg. If false throws logic_error with a_failmsg.
 * 
 * \pre a_msg /= nul and a_failmsg /= nul
 */
inline void base_test::bool_test(bool a_test, std::string a_msg)

{
  COMPCHEM_REQUIRE (not a_msg.empty (), "bool_test requires a non-empty message");
  if (a_test)
  {
    log () << IFYS << "   passed   : " << a_msg << std::endl;
  }
  else
  {
    COMPCHEM_ALWAYS (a_test, IFNO + " **failed** : " + a_msg);
  }

}

/**
 * If a_test is true print a_msg. If false throws logic_error with a_failmsg.
 * 
 * \pre a_msg /= nul and a_failmsg /= nul
 */
inline void base_test::bool_test(bool a_test, const char *const a_msg)
{
  if (NULL != a_msg)
  {
    if (a_test)
    {
      log () << IFYS << "   passed   : " << a_msg << std::endl;
    }
    else
    {
      COMPCHEM_ALWAYS (a_test, IFNO + " **failed** : " + a_msg);
    }
  }
  else
  {
    COMPCHEM_ALWAYS (a_test, IFNO + " **failed** : bool_test (no message)");
    log () << IFYS << "   passed   : bool_test (no message)" << std::endl;
  }

}

/**
 * This class provides the application initialisation and control system for a test suite application
 */
class test_manager {
  public:
    typedef std::vector<base_test* >::iterator iterator;

    typedef std::vector<base_test* >::const_iterator const_iterator;


  private:
    /**
     * The single test_manager instance.
     */
    static test_manager * s_exemplar;

    /**
     * Pointer to the stream to use for the log.
     */
    mutable std::ostream * m_log;

    /**
     * An optional stream for the log messages.
     */
    mutable std::auto_ptr<std::ostream> m_log_stream;

    /**
     * The test routines
     */
    std::vector< base_test * > m_test_routines;


  public:
    /**
     * Add a test function to suite.
     * 
     * \pre not has_tester (a_fntor)
     * \post has_tester (a_fntor) and new_size = old_size + 1
     */
    inline void add_tester(base_test * a_fntor);

    /**
     * Is there any registered unit-tests?
     */
    bool empty() const
    {
      return m_test_routines.empty ();
    }

    /**
     * Get a pointer to the manager exemplar object
     * 
     * \post result != NULL
     */
    static  test_manager& get_exemplar() throw()
    {
      if (NULL == s_exemplar)
      {
        s_exemplar = new test_manager ();
      }
      return *s_exemplar;
    }

    /**
     * Is a_tstr in our test set?
     */
    inline bool has_tester(const base_test * a_tstr);

    /**
     * Logging stream used by all unit tests.
     */
    std::ostream & log() const
    {
      return *m_log;
    }

    /**
     * Routine to manage unit tests.
     * 
     * Note: base_test derived classes indicate failure by an uncaught
     * exception.
     */
    int main(int argc, char ** argv) const;

    /**
     * Remove a test function to suite.
     * 
     * \post size() == old.size() + 1
     */
    inline void remove_tester(const base_test * a_fntor);

    unsigned int size() const
    {
      return m_test_routines.size ();
    }

  private:
    /**
     * Constuctor is only used by the test_manager exemplar. It does nothing.
     */
    test_manager()
    : m_log (&std::cout)
    , m_log_stream ()
    , m_test_routines()
    {}


  public:
    ~test_manager() {}

    std::string title() const
    {
      return "Test manager";
    }
    /**
     * Switch to using this a_fname as file for log messages.
     */
    void use_log_file(const std::string& a_fname) const;


  private:
    /**
     * no copy
     */
    test_manager(const test_manager & source);

    /**
     * no assign
     */
    test_manager & operator=(const test_manager & source);

};
/**
 * Add a test function to suite.
 * 
 * \pre not has_tester (a_fntor)
 * \post has_tester (a_fntor) and new_size = old_size + 1
 */
inline void test_manager::add_tester(base_test * a_fntor) 
{
  COMPCHEM_REQUIRE(not has_tester (a_fntor), "Object already present cannot add tester object twice.");
  COMPCHEM_ENSURE_OLD(const unsigned int l_oldsize (m_test_routines.size ()));
  m_test_routines.push_back (a_fntor);
  COMPCHEM_ENSURE(has_tester (a_fntor), "Object not in list, adding tester object failed.");
  COMPCHEM_ENSURE(m_test_routines.size () == 1 + l_oldsize, "List is wrong size, adding tester object failed.");
}

/**
 * Is a_tstr in our test set?
 */
inline bool test_manager::has_tester(const base_test * a_tstr) 
{
  return (m_test_routines.end () != std::find (m_test_routines.begin (), m_test_routines.end (), a_tstr));
}

/**
 * Remove a test function to suite.
 * 
 * \post size() == old.size() + 1
 */
inline void test_manager::remove_tester(const base_test * a_fntor) {
  COMPCHEM_REQUIRE(true == has_tester (a_fntor), "Cannot remove tester object that is not in this object.");
  m_test_routines.erase (std::find (m_test_routines.begin (), m_test_routines.end (), a_fntor));
  COMPCHEM_ENSURE(false == has_tester (a_fntor), "Removing tester object failed."); }

} // namespace utility
#endif
