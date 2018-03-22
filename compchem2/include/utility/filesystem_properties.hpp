#ifndef FILESYSTEM_PROPERTIES_HPP
#define FILESYSTEM_PROPERTIES_HPP

// ------------------------------------------------------------
// class: filesystem_properties
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
// Std C++ includes
#include <cerrno>
// Selective includes for this type.
#ifdef HAVE_UNISTD_H
extern "C"
{
# include <libgen.h>
# include <sys/types.h>
# include <unistd.h>
# include <pwd.h>
}
#else
#error Need POSIX - define HAVE_UNISTD_H if present
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#else
#error Need stat - define HAVE_SYS_STAT_H if present
#endif

#include <string>
#include "utility/contract_error.hpp"
#include "utility/epoch_time.hpp"

namespace utility {

/**
 * Access important POSIX properties relating to files. This provides access to the set of file properties from real POSIX compliant file system object.  It also provides convenience functions for accessing POSIX properties relating to files.
 */
class filesystem_properties {
  public:
    /**
     * Get working directory name
     * 
     * \post Result may be an empty string.
     * \return An empty string indicates failure.
     */
    static std::string current_directory();

    static std::string current_user()
    {
      const char* const l_tmp = getenv ("LOGNAME");
      return (NULL != l_tmp ? std::string (l_tmp) : std::string ());
    }

    /**
     * Get the processes effective user ID
     */
    static inline uid_t effective_user() throw() { return geteuid(); }

    /**
     * Get the processes effective group ID
     */
    static inline gid_t effective_group() throw() { return getegid(); }

    /**
     * Is there a corresponding file?
     * 
     * \post exists() implies m_stat != NULL
     */
    bool exists() const throw()
    {
      return (m_stat != NULL);
    }

    /**
     * Get effective user's home directory name
     * 
     * \post Return of empty string indicates an error.
     */
    static std::string home_directory();

    /**
     * Is the file system object a block device?
     * 
     * \pre exists()
     */
    bool is_block_device() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (0 != S_ISBLK(m_stat->st_mode));
    }

    /**
     * Is the file system object a character device?
     * 
     * \pre exists()
     */
    inline bool is_character_device() const throw(contract_error)
    {
      REQUIRE (exists (), "Cannot get property of non-existant file");
      return (0 != S_ISCHR(m_stat->st_mode));
    }

    /**
     * Is the file system object a directory?
     * 
     * \pre exists()
     */
    bool is_directory() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (0 != S_ISDIR(m_stat->st_mode));
    }

    /**
     * Can the effective user read/execute this file object?
     * 
     * \pre exists()
     */
    bool is_executable(uid_t a_user, gid_t a_group) const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (S_IXOTH == (S_IXOTH & m_stat->st_mode))
    	|| (S_IXGRP == (S_IXGRP & m_stat->st_mode) && a_group == m_stat->st_gid)
    	|| (S_IXUSR == (S_IXUSR & m_stat->st_mode) && a_user == m_stat->st_uid);
    }

    /**
     * Is the file system object related a regular file?
     * 
     * \pre exists()
     */
    bool is_file() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (0 != S_ISREG(m_stat->st_mode));
    }

    /**
     * Is the file system object a POSIX message queue?
     * 
     * \pre exists()
     */
    bool is_message_queue() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (0 != S_TYPEISMQ(m_stat));
    }

    /**
     * Can the effective user open a file with the given filename for reading.
     * 
     * The filename can be relative or absolute.
     * 
     * \returns A pair of bools.  The first indicates if the file existed at the time the FS was queried.  If the first is true then the second indicates if the file can be openned.
     */
    static inline std::pair< bool, bool> is_openable(const std::string & a_file_name)
    {
      return is_openable_p (a_file_name, false);
    }


  private:
    /**
     * Test if file can be openned either for reading or writing.
     * 
     * This tests existing files, where it checks the permissions of the file.
     * 
     * \param a_to_write If false only check if readable.  If true check if writable.
     * 
     * \returns A pair of bools.  The first indicates if the file existed at the time the FS was queried.  If the first is true then the second indicates if the file is openable/writable depending on a_to_write
     */
    static inline std::pair<bool, bool> is_openable_p(const std::string & a_file_name, bool a_to_write)
    {
      filesystem_properties l_prop (a_file_name, true);
      std::pair<bool, bool> Result (l_prop.exists (), false);
      if (Result.first)
      {
        if (a_to_write)
        {
          Result.second = l_prop.is_writable (effective_user (), effective_group ());
        }
        else
        {
          Result.second = l_prop.is_readable (effective_user (), effective_group ());
        }
      }
      return Result;
    }

  public:
    /**
     * Is the file system object a pipe/fifo?
     * 
     * \pre exists()
     */
    bool is_pipe() const throw(contract_error)
    {
      COMPCHEM_REQUIRE (exists (), "Cannot get property of non-existant file");
      return (0 != S_ISFIFO(m_stat->st_mode));
    }

    /**
     * Can the effective user read this file object?
     * 
     * \pre exists()
     */
    bool is_readable(const uid_t a_user, const gid_t a_group) const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (S_IROTH == (S_IROTH & m_stat->st_mode))
    	|| (S_IRGRP == (S_IRGRP & m_stat->st_mode) && a_group == m_stat->st_gid)
    	|| (S_IRUSR == (S_IRUSR & m_stat->st_mode) && a_user == m_stat->st_uid);
    }

    /**
     * Is the file system object a POSIX semaphore?
     * 
     * \pre exists()
     */
    bool is_semaphore() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (0 != S_TYPEISSEM(m_stat));
    }

    /**
     * Is the file system object a POSIX shared mempry?
     * 
     * \pre exists()
     */
    bool is_shared_memory() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (0 != S_TYPEISSHM(m_stat));
    }

    /**
     * Is the file system object a socket?
     * 
     * \pre exists()
     */
    bool is_socket() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (0 != S_ISSOCK(m_stat->st_mode));
    }

    /**
     * Is the file system object a symlink (may be "dangling")
     * 
     * \pre exists()
     */
    bool is_symlink() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (0 != S_ISLNK(m_stat->st_mode));
    }

    /**
     * Can the effective user write to this file object?
     * 
     * \pre exists()
     */
    bool is_writable(const uid_t a_user, const gid_t a_group) const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return (S_IWOTH == (S_IWOTH & m_stat->st_mode))
    	|| (S_IWGRP == (S_IWGRP & m_stat->st_mode) && a_group == m_stat->st_gid)
    	|| (S_IWUSR == (S_IWUSR & m_stat->st_mode) && a_user == m_stat->st_uid);
    }

    /**
     * Can the effective user open an existing file with the given filename for writing.
     * 
     * The filename can be relative or absolute. 
     * 
     * \returns A pair of bools.  The first indicates if the file existed at the time the FS was queried.  If the first is true then the second indicates if the file is writable
     */
    static inline std::pair< bool, bool> is_writable(const std::string & a_file_name)
    {
      return is_openable_p (a_file_name, true);
    }

    /**
     * Get the size (in bytes) of a file or symlink.
     * 
     * \pre exists()
     */
    unsigned int size() const throw(contract_error)
    {
      COMPCHEM_REQUIRE (exists (), "Cannot get size of non-existant file");
      return m_stat->st_size;
    }

    /**
     * Time of last call by execve(2), mknod(2), pipe(2), utime(2) or read(2).
     * 
     * \pre exists()
     */
     epoch_time time_access() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return epoch_time (m_stat->st_atime);
    }

    /**
     * Time set by last write or changed inode information (i.e., owner, group, link count, mode, etc.).
     * 
     * \pre exists()
     */
    epoch_time time_change() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return epoch_time (m_stat->st_ctime);
    }

    /**
     * Time of last file modifications, e.g. by mknod(2), truncate(2), utime(2) and write(2) (of more than zero bytes). For a directory; last creation or deletion of files.
     * 
     * \pre exists()
     */
    epoch_time time_modification() const throw(contract_error)
    {
      COMPCHEM_REQUIRE(exists (), "Cannot get property of non-existant file");
      return epoch_time (m_stat->st_mtime);
    }

    /**
     * Get properties of named file system object.
     * 
     * \param a_filename The absolute or relative name of a file system object.
     * 
     * \param a_follow_link Optional argument to indicate differential behavior with symlinks. The default (false) behavior is to examine properties of a symlilnk.  When true, the properties of the linked-to file are examined. (This only works if compiled with when __USE_BSD or __USE_XOPEN_EXTENDED are defined; otherwise this is silently ignored.)
     * 
     * \pre !a_filename.empty()
     * 
     * \throw DBC_EXCEPT and from new operator
     */
    inline filesystem_properties(const std::string & a_filename, bool a_follow_link = false)
    : m_stat (new struct stat)
    {
      COMPCHEM_REQUIRE(!a_filename.empty(), "Cannot get properties of file without name");
      int l_exists;
#if (defined __USE_BSD || defined __USE_XOPEN_EXTENDED)
      if (a_follow_link)
      {
        l_exists = stat (a_filename.c_str (), m_stat);
      }
      else
      {
        l_exists = lstat (a_filename.c_str (), m_stat);
      }
#else
      l_exists = stat (a_filename.c_str (), m_stat);
#endif
      if (0 != l_exists)
      {
        delete m_stat;
        m_stat = NULL;
      }
    }
    /**
     * \brief Destructor
     */
    ~filesystem_properties() throw()
    {
      if (exists ())
      {
        delete m_stat;
      }
    }

    /**
     * Default constructor
     * 
     * \post exists() = false
     */
    filesystem_properties()
    : m_stat (NULL)
    {}

    /**
     * Copy constructor
     * 
     * \may throw from new
     */
    filesystem_properties(const filesystem_properties & a_prop)
    : m_stat (NULL)
    {
      if (a_prop.exists ())
      {
        m_stat = new struct stat;
        memcpy(m_stat, a_prop.m_stat, sizeof(struct stat));
      }
    }

    /**
     * Copy operator
     */
    const filesystem_properties & operator =(const filesystem_properties & a_prop)
    {
      if (this != &a_prop)
      {
        filesystem_properties l_tmp (a_prop);
        swap (l_tmp);
      }
      return *this;
    }

    /**
     * Swap state of this with a_other.
     * 
     * strong throw guaranttee
     */
    void swap(filesystem_properties & a_other);


  private:
    struct stat * m_stat;

};

} // namespace utility
#endif
