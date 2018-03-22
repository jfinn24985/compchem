#ifndef COMPCHEM_CONTRACT_ERROR_HPP
#define COMPCHEM_CONTRACT_ERROR_HPP

// ------------------------------------------------------------
// class: contract_error
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "requirements.hpp"
#include <exception>

#include <string>

#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif
namespace utility {

/**
 * A Design-by-contract violation exception. The contracts
 * supported are: always, require, ensure, invariant and check.
 * The violation severity follows the order listed above. One of 
 * the template parameters specifies what the maximum test
 * level will be.
 */
class contract_error : public std::exception {
  public:
    /**
     * ID values for the contract types
     */
    enum {
      always= 0,/**
       * An 'always' contract is a utility contract for error conditions that must always
       * be tested.  It is not really a 
       */

      require= 1,/**
       * A require contract is one that must be met on entry into a method. It's
       * purpose is to guard the state of an object from invalid input.
       */

      ensure= 2,/**
       * An ensure contract is tested on exiting a function.  Its purpose is 
       * to ensure that a method's result is within some tolerance.Throws
       * std::domain_error
       */

      invariant= 3,/**
       * An invariant contract is used on function entry and exit to
       * make sure that the object is in a valid state.
       */

      check= 4,/**
       * A check contract is used to check that methods and the programming
       * logic leading up to the check have operated as expected and contracted.
       * 
       * For example:
       * T *p = new T;
       * COMPCHEM_CHECK(NULL != p, "New should never return a null pointer!");
       */

      max_level= 5,/**
       * The maximum reporting level (0-4).  For which errors
       * are reported.
       */

      s_size= 4000/**
       * The buffer size.
       */


    };

    /**
     * The contract error level.
     */
    const unsigned int level;


  private:
    /**
     * The error message
     */
    char m_what[s_size];


  public:
    /**
     * Create a contract exception with a formatted output. Primarily designed to 
     * operate with the Design-by-Contract macros. This class doesn't allocate 
     * memory from the heap.
     */
     contract_error(const std::string & a_msg1, const char * a_msg2, unsigned int a_level, const char * a_filename, int a_line, const char * a_fn_name, void * const * a_backtrace=NULL, int a_backsz=0) throw() ;

    contract_error(const contract_error & a_other);

    ~contract_error() throw ()
    {}

    /**
     * The error message
     */
    const char * what() const throw ()
    {
      return &m_what[0];
    }

    contract_error & operator=(const contract_error & a_other);
    /**
     * throw-less swap.
     */
    void swap(contract_error & a_other);

};

} // namespace utility

// Attempt to provide a meaningful function name on systems that support it.
// This paragraph attempts to use the GNC gcc or standard C names before
// falling back to nothing.  If a better choice is known you can predefine
// this macro.
#ifndef COMPCHEM_FUNCTION_NAME
# if defined __cplusplus && __GNUC_PREREQ(2, 6)
#   define COMPCHEM_FUNCTION_NAME __PRETTY_FUNCTION__
# else
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#   define COMPCHEM_FUNCTION_NAME __func__
#  else
#   define COMPCHEM_FUNCTION_NAME ((const char *) 0)
#  endif
# endif
#endif

/// Macro to test for error conditions.  This macro is used to test
/// for conditions that must always be met but are outside the
/// programmes control. An example might be testing the returned value
/// of malloc() where a nul pointer is rare (these days) but possible.
///
/// This macro prints an error message and calls std::abort() to end the program
#ifdef HAVE_BACKTRACE
#define COMPCHEM_TEST(LVL, X, Y) {\
  if (LVL <= ::utility::contract_error::max_level and !(X)) { \
    void * TRACE[20];                                            \
    const int l_sz = backtrace (&TRACE[0], 20);                  \
    throw ::utility::contract_error (Y, __STRING(X), LVL, __FILE__, __LINE__, COMPCHEM_FUNCTION_NAME, &TRACE[0], l_sz);\
  }\
}
#else
#define COMPCHEM_TEST(LVL, X, Y) {\
  if (LVL <= ::utility::contract_error::contract_error::max_level and !(X)) {\
    throw ::utility::contract_error (Y, __STRING(X), LVL, __FILE__, __LINE__, COMPCHEM_FUNCTION_NAME, NULL, 0);\
  }\
}
#endif

#define COMPCHEM_ALWAYS(X,Y) COMPCHEM_TEST(::utility::contract_error::always,(X),(Y))

#ifndef DbC_LEVEL
#define DbC_LEVEL 4
#endif

/// Macro to test for required conditions or preconditions. These are
/// conditions that must be met before a method starts.
#if defined (NO_DbC) || DbC_LEVEL < 1 
#define REQUIRE(X, Y)
#define COMPCHEM_REQUIRE(X, Y)
#else
#define REQUIRE(X,Y) COMPCHEM_TEST(::utility::contract_error::require,(X),(Y))
#define COMPCHEM_REQUIRE(X,Y) COMPCHEM_TEST(::utility::contract_error::require,(X),(Y))
#endif

/// Macro to test check conditions. This tests conditions that the
/// programmer thinks should be true based on the preceding logic.
#if defined (NO_DbC) || DbC_LEVEL < 4 
#define CHECK(X, Y)
#define COMPCHEM_CHECK(X, Y)
#else
#define CHECK(X,Y) COMPCHEM_TEST(::utility::contract_error::check,(X),(Y))
#define COMPCHEM_CHECK(X,Y) COMPCHEM_TEST(::utility::contract_error::check,(X),(Y))
#endif

/// Macro to test for ensured conditions or postconditions. These are
/// conditions that the method should ensure are met at the end of a
/// method.
#if defined (NO_DbC) || DbC_LEVEL < 2 
#define ENSURE(X, Y)
#define COMPCHEM_ENSURE(X, Y)
#else
#define ENSURE(X,Y) COMPCHEM_TEST(::utility::contract_error::ensure,(X),(Y))
#define COMPCHEM_ENSURE(X,Y) COMPCHEM_TEST(::utility::contract_error::ensure,(X),(Y))
#endif
/// Helper macro for ENSURE, allows definition of `old' values at
/// start of method that can be used by ENSURE. For example it might
/// be used to define an int that stores the size of a container,
/// which is compared to the size of a container after the method to
/// see if items have been successfully added/removed etc.
#if defined (NO_DbC) || DbC_LEVEL < 2 
#define ENSURE_OLD(X)
#define COMPCHEM_ENSURE_OLD(X)
#else
#define ENSURE_OLD(X) X
#define COMPCHEM_ENSURE_OLD(X) X
#endif
/// Macro to test invariant conditions. These conditions are ones that
/// should always be met by an object. They should be tested at the
/// entry of all methods. They should also be tested at the exit of
/// any non-const method.
#if defined (NO_DbC) || DbC_LEVEL < 3
#define INVARIANT(X, Y)
#define COMPCHEM_INVARIANT(X, Y)
#else
#define INVARIANT(X,Y) COMPCHEM_TEST(::utility::contract_error::invariant,(X),(Y))
#define COMPCHEM_INVARIANT(X,Y) COMPCHEM_TEST(::utility::contract_error::invariant,(X),(Y))
#endif

#endif
