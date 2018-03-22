// ------------------------------------------------------------
// class: test_filesystem_util
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/utility_test/test_filesystem_util.hpp"
#include "utility/program_options.hpp"

#include <fstream>
#include <iostream>
namespace utility {

test_filesystem_util test_filesystem_util::exemplar;

/**
 * Override in derived classes to perform unit test.
 */
void test_filesystem_util::main_test() const {
  boost::filesystem::path l_path1 ("/etc/path1");
  boost::filesystem::path l_path2 ("path2/a");
  boost::filesystem::path l_path3 ("/etc/path3/a");
  log () << COMM << "Path 1 = " << l_path1.string () << "\n";
  log () << COMM << "Path 2 = " << l_path2.string () << "\n";
  log () << COMM << "Path 3 = " << l_path3.string () << "\n";
  boost::filesystem::path l_result;
  l_result = utility::filesystem::standardize (l_path1);
  log () << COMM << "Standardize (Path 1) = " << l_result.string () << "\n";
  bool_test (l_path1 == l_result, "Path 1 should be standard.");
  l_result = utility::filesystem::standardize (l_path2);
  log () << COMM << "Standardize (Path 2) = " << l_result.string () << "\n";
  bool_test (l_path2 != l_result, "Path 2 should not be standard.");
  l_result = utility::filesystem::relative_path (l_path1, l_path3);
  log () << COMM << "Relative (Path 1, Path 3) = " << l_result.string () << "\n";
  bool_test (utility::filesystem::standardize (l_result) != l_result, "Relative path should not be standard.");
  l_result = utility::filesystem::standardize (l_result);
  log () << COMM << "The following makes no real sense!\n";
  log () << COMM << "Standardize (Relative (Path 1, Path 3)) = " << l_result.string () << "\n";
  bool_test (l_result != l_path3, "Standardized Relative path should not be Path 3.");
  bool_test (l_result != l_path1, "Standardized Relative path should not be Path 1.");
  l_result = l_path1 / utility::filesystem::relative_path (l_path1, l_path3);
  log () << COMM << "Path 1 / (Relative (Path 1, Path 3)) = " << l_result.string () << "\n";
  bool_test (utility::filesystem::standardize (l_result) == l_path3, "Standardized (Path 1 / Relative path) should be Path 3.");
  
  bool_test (boost::filesystem::exists (m_compare_fname), "Test file [" + m_compare_fname + "] exists.");
  std::ifstream l_ifs (m_compare_fname.c_str ());
  std::string l_buf;
  std::getline (l_ifs, l_buf, (char)std::char_traits<char>::eof ());
  bool_test (not l_buf.empty (), "Read in test file.");
  bool_test (utility::filesystem::compare (m_compare_fname, l_buf.data (), l_buf.size ()), "Read buffer and disk file are identical");
  
  //  boost::filesystem::path l_dos1 ("a:/dir1", &boost::filesystem::windows_name);
  //  boost::filesystem::path l_dos2 ("b:/dir1", &boost::filesystem::windows_name);
  //  boost::filesystem::path l_dos3 ("//share/dir1", &boost::filesystem::windows_name);
  //  boost::filesystem::path l_dos4 ("//share/dir2", &boost::filesystem::windows_name);
  //  log () << COMM << "DOS Path 1 = " << l_dos1.string () << "\n";
  //  log () << COMM << "DOS Path 2 = " << l_dos2.string () << "\n";
  //  log () << COMM << "DOS Path 3 = " << l_dos3.string () << "\n";
  //  log () << COMM << "DOS Path 4 = " << l_dos4.string () << "\n";
  //  l_result = utility::filesystem::standardize (l_dos1);
  //  log () << COMM << "Standardize (DOS Path 1) = " << l_result.string () << "\n";
  //  bool_test (l_path1 == l_result, "DOS Path 1 should be standard.");
  //  l_result = utility::filesystem::standardize (l_dos3);
  //  log () << COMM << "Standardize (DOS Path 3) = " << l_result.string () << "\n";
  //  bool_test (l_path1 == l_result, "DOS Path 3 should be standard.");
  //  l_result = utility::filesystem::relative_path (l_dos1, l_dos2);
  //  log () << COMM << "Relative path (DOS Path 1 and 2) = " << l_result.string () << "\n";
  //  bool_test (l_result.empty (), "Should be no relative path.");
  //  l_result = utility::filesystem::relative_path (l_dos1, l_dos3);
  //  log () << COMM << "Relative path (DOS Path 1 and 3) = " << l_result.string () << "\n";
  //  bool_test (l_result.empty (), "Should be no relative path.");
  //  l_result = utility::filesystem::relative_path (l_dos3, l_dos4);
  //  log () << COMM << "Relative path (DOS Path 3 and 4) = " << l_result.string () << "\n";
  //  bool_test (not l_result.empty (), "Should be have relative path.");
  //  bool_test (utility::filesystem::standardize (l_result) != l_result, "Relative path should not be standard.");
  //  l_result = utility::filesystem::standardize (l_result);
  //  log () << COMM << "Standardize (Relative (DOS Path 3 and 4)) = " << l_result.string () << "\n";
  //  bool_test (l_result == l_dos3, "Standardized Relative path should be Path 3.");
  //  bool_test (l_result == l_dos4, "Standardized Relative path should be Path 4.");
}

/**
 * This method is called by the manager to allow test types to 
 * register program options.  The default implementation does
 * nothing.
 */
void test_filesystem_util::register_options() 
{
  // Add option
  utility::program_options::helper ().public_options ().add_options () ("compare-file", boost::program_options::value< std::string >(&this->m_compare_fname), "Test file for 'compare' function");

}


} // namespace utility
