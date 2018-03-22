// ------------------------------------------------------------
// class: test_posix
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/utility_test/test_posix.hpp"
#include "utility/epoch_time.hpp"
#include "utility/program_options.hpp"
#include "utility/filesystem_properties.hpp"

namespace utility {

/**
 * Create a static instance to register this set of test cases.
 */
test_posix test_posix::s_exemplar;

/**
 * Perform unit test on POSIX helpers
 */
void test_posix::main_test() const {
  // LOCAL
  class print
  {
  public:
    static const char *at (bool a_is)
    {
      return a_is ? "Yes" : "No";
    };
    static const char *at (std::pair < bool, bool > a_is)
    {
      return a_is.first ? (a_is.second ? "Yes" : "No") : "Doesn't exist?";
    };
  };
  
  // DO
  if (not m_filelist.empty ())
  {
    const std::string l_fn1 (*m_filelist.begin ());
    const filesystem_properties l_fp1 (*m_filelist.begin ());
    for (std::vector < std::string >::const_iterator l_i (m_filelist.begin ())
         ; l_i != m_filelist.end (); ++l_i)
    {
      filesystem_properties l_fp2 (*l_i);
      print_file_properties (*l_i, l_fp2, log ());
      if (l_fp2.exists () && l_fp1.exists ())
      {
        log () << WTCH << "--- [" << l_fn1 << "] cf. [" << *l_i << "]\n";
        log () << TTLE << "epoch_time exercises.\n";
        const epoch_time l_t1 (l_fp1.time_change ());
        epoch_time l_t2;
        l_t2 = l_fp2.time_change ();
        log () << WTCH << " last change (<) : " << print::at (l_t1 < l_t2) << "\n";
        log () << WTCH << " last change (>) : " << print::at (l_t1 > l_t2) << "\n";
        log () << WTCH << " last change (<=)  : " << print::at (l_t1 <= l_t2) << "\n";
        log () << WTCH << " last change (>=)  : " << print::at (l_t1 >= l_t2) << "\n";
        log () << WTCH << " last change (==)  : " << print::at (l_t1 == l_t2) << "\n";
        log () << WTCH << " last change (!=)  : " << print::at (l_t1 != l_t2) << "\n";
        log () << WTCH << " last change (is_same)  : " << print::at (l_t1.is_same (l_t2)) << "\n";
        log () << WTCH << " last change (is_earlier)  : " << print::at (l_t1.is_earlier (l_t2)) << "\n";
      }
      log () << WTCH << " File " << *l_i << " is openable : " << print::at (filesystem_properties::is_openable (*l_i)) << "\n";
      log () << WTCH << " File " << *l_i << " is writable : " << print::at (filesystem_properties::is_writable (*l_i)) << "\n";
    }
  }
  log () << WTCH << "=== General Filesystem Properties ===\n";
  log () << WTCH << " Effective User ID is " << filesystem_properties::effective_user () << "\n";
  log () << WTCH << " Effective Group ID is " << filesystem_properties::effective_group () << "\n";
  log () << WTCH << " Effective User Home directory is " << filesystem_properties::home_directory () << "\n";
  log () << WTCH << " Current working directory is " << filesystem_properties::current_directory () << "\n";
  log () << TTLE << "Unit tests.\n";
  filesystem_properties l_fp3;
  log () << WTCH << "Default ctor () object exists ? " << print::at (l_fp3.exists ()) << "\n";
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_block_device, &l_fp3)
      , "Attempt to check if block device on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_character_device, &l_fp3)
      , "Attempt to check if character device on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_directory, &l_fp3)
      , "Attempt to check if directory on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_executable, &l_fp3, filesystem_properties::effective_user (), filesystem_properties::effective_group ())
      , "Attempt to check if executable on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_file, &l_fp3)
      , "Attempt to check if file on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_message_queue, &l_fp3)
      , "Attempt to check if message_queue on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_pipe, &l_fp3)
      , "Attempt to check if pipe on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_readable, &l_fp3, filesystem_properties::effective_user (), filesystem_properties::effective_group ())
      , "Attempt to check if readable on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_semaphore, &l_fp3)
      , "Attempt to check if semaphore on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_shared_memory, &l_fp3)
      , "Attempt to check if shared_memory on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_socket, &l_fp3)
      , "Attempt to check if socket on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_symlink, &l_fp3)
      , "Attempt to check if symlink on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::is_writable, &l_fp3, filesystem_properties::effective_user (), filesystem_properties::effective_group ())
      , "Attempt to check if writable on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::size, &l_fp3)
      , "Attempt to get size on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::time_access, &l_fp3)
      , "Attempt to get time_access on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::time_change, &l_fp3)
      , "Attempt to get time_change on non-existant node.");
  monitor_function (contract_error::require
      , boost::bind (&filesystem_properties::time_modification, &l_fp3)
      , "Attempt to get time_modification on non-existant node.");
}

/**
 * Pretty print the file properties to a_stream.
 */
void test_posix::print_file_properties(const std::string& a_filename, const filesystem_properties & a_props, std::ostream & a_os) const {
  class print
  {
  public:
    static const char *const as (bool a_test)
    {
      return ((a_test) ? " Y\n" : "    N\n");
    }
  };
  
  a_os << COMM << "======= PROPERTIES =======\n";
  a_os << WTCH << "FILENAME [" << a_filename << "]\n";
  a_os << WTCH << "exists          :" << print::as (a_props.exists ());
  if (a_props.exists ())
  {
    uid_t
    l_user (filesystem_properties::effective_user ());
    gid_t
    l_group (filesystem_properties::effective_group ());
    a_os << COMM << "------- file type --------\n";
    a_os << WTCH << "block dev.      :" << print::as (a_props.is_block_device ());
    a_os << WTCH << "char. dev.      :" << print::as (a_props.is_character_device ());
    a_os << WTCH << "directory       :" << print::as (a_props.is_directory ());
    a_os << WTCH << "ordinary file   :" << print::as (a_props.is_file ());
    a_os << WTCH << "message queue   :" << print::as (a_props.is_message_queue ());
    a_os << WTCH << "pipe            :" << print::as (a_props.is_pipe ());
    a_os << WTCH << "semaphore       :" << print::as (a_props.is_semaphore ());
    a_os << WTCH << "shared memory   :" << print::as (a_props.is_shared_memory ());
    a_os << WTCH << "socket          :" << print::as (a_props.is_socket ());
    a_os << WTCH << "symbolic link   :" << print::as (a_props.is_symlink ());
    a_os << COMM << "---- other properties -----\n";
    a_os << WTCH << "size            :" << a_props.size () << "\n";
    a_os << WTCH << "access time     :" << std::string (a_props.time_access ());
    a_os << WTCH << "change time     :" << std::string (a_props.time_change ());
    a_os << WTCH << "modification    :" << std::string (a_props.time_modification ());
    a_os << COMM << "- rights of effective user \n";
    a_os << WTCH << "can open/read   :" << print::as (a_props.is_readable (l_user, l_group));
    a_os << WTCH << "can open/write  :" << print::as (a_props.is_writable (l_user, l_group));
    a_os << WTCH << "can execute     :" << print::as (a_props.is_executable (l_user, l_group));
  }
  a_os << COMM << "--------------------------\n";
}

/**
 * This method is called by the manager to allow test types to 
 * register program options.  The default implementation does
 * nothing.
 */
void test_posix::register_options() 
{
  if (NULL == utility::program_options::helper ().public_options ().find_nothrow ("file", true))
  {
    // Add option
    utility::program_options::helper ().public_options ().add_options ()
    ("file", boost::program_options::value < std::vector < std::string > >(&(this->m_filelist))->composing (), "Files to test")
    ("posix-ignore", boost::program_options::value < std::string >(), "Regular expression of variable items to exclude from canonical comparison");
  }

}


} // namespace utility
