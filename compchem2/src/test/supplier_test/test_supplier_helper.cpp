// ------------------------------------------------------------
// class: test_supplier_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/supplier_test/test_supplier_helper.hpp"
#include <boost/bind.hpp>

#include "utility/scoped_call.hpp"
#include "supplier/supplier_helper.hpp"

namespace supplier {

/**
 * Class exemplar
 */
test_supplier_helper test_supplier_helper::s_exmplr;

/**
 * Override in derived classes to perform unit test.
 */
void test_supplier_helper::main_test() const {
#line 100
  supplier::supplier_helper & l_help (supplier::supplier_helper::helper ());
  unsigned int l_dir_count (0);
  if (l_help.begin_directory () != l_help.end_directory ())
  {
    log () << COMM << "The following plugin library locations defined.\n";
    for (supplier::supplier_helper::dir_iterator l_i (l_help.begin_directory ()); l_i != l_help.end_directory (); ++l_i)
    {
      log () << WTCH << "  " << *l_i << "\n";
      ++l_dir_count;
    }
  }
  else
  {
    log () << COMM << "No plugin library locations defined.\n";
  }
  const std::string l_cmpd_orig (l_help.target_compound ());
  const std::string l_expt_orig (l_help.target_experiment ());
  const std::string l_file_orig (l_help.target_file ().string ());
  const std::string l_form_orig (l_help.target_formula ());
  const std::string l_memo_orig (l_help.target_memo ());
  const std::string l_mems_orig (l_help.target_memo_src ());
  /////////////////
  // Set up to reset helper.
  utility::scoped_call l_cmpd_reset (boost::bind (&supplier::supplier_helper::target_compound
                                                  , &l_help, l_cmpd_orig));
  utility::scoped_call l_expt_reset (boost::bind (&supplier::supplier_helper::target_experiment
                                                  , &l_help, l_expt_orig));
  utility::scoped_call l_file_reset (boost::bind (&supplier::supplier_helper::target_file
                                                  , &l_help, l_file_orig));
  utility::scoped_call l_form_reset (boost::bind (&supplier::supplier_helper::target_formula
                                                  , &l_help, l_form_orig));
  utility::scoped_call l_memo_reset (boost::bind (&supplier::supplier_helper::target_memo
                                                  , &l_help, l_memo_orig));
  utility::scoped_call l_mems_reset (boost::bind (&supplier::supplier_helper::target_memo_src
                                                  , &l_help, l_mems_orig));
  
  if (l_cmpd_orig.empty ())
  {
    log () << COMM << "No target compound label.\n";
  }
  else
  {
    log () << COMM << "Target compound label: " << l_cmpd_orig << ".\n";
  }
  if (l_expt_orig.empty ())
  {
    log () << COMM << "No target experiment reference.\n";
  }
  else
  {
    log () << COMM << "Target experiment reference: " << l_expt_orig << ".\n";
  }
  if (l_file_orig.empty ())
  {
    log () << COMM << "No data file given.\n";
  }
  else
  {
    log () << COMM << "Data file name is :" << l_file_orig << ".\n";
  }
  if (l_form_orig.empty ())
  {
    log () << COMM << "No target formula label.\n";
  }
  else
  {
    log () << COMM << "Target formula label: " << l_form_orig << ".\n";
  }
  if (l_memo_orig.empty ())
  {
    log () << COMM << "No target memo label.\n";
  }
  else
  {
    log () << COMM << "Target memo label: " << l_memo_orig << ".\n";
  }
  if (l_mems_orig.empty ())
  {
    log () << COMM << "No target memo src label.\n";
  }
  else
  {
    log () << COMM << "Target memo src label: " << l_mems_orig << ".\n";
  }
  log () << TTLE << "Test setting options.\n";
  ////////////////////
  // Test adding a dir
  const std::string l_dir ("/home/finnerty/test.d/");
  l_help.add_directory (l_dir);
  bool_test (l_help.begin_directory () != l_help.end_directory (), "begin /= end after adding directory");
  {
    unsigned int l_count (0);
    unsigned int l_new_count (0);
    log () << COMM << "The following plugin library locations defined.\n";
    for (supplier::supplier_helper::dir_iterator l_i (l_help.begin_directory ()); l_i != l_help.end_directory (); ++l_i)
    {
      log () << WTCH << "  " << *l_i << "\n";
      if (l_dir == *l_i)
      {
        ++l_count;
      }
      ++l_new_count;
    }
    bool_test (1 == l_count, "found added directory \"" + l_dir + "\" once");
    bool_test (l_dir_count + 1 == l_new_count, "one directory entry added");
  }
  ////////////////////////////
  // Test setting the compound
  const std::string l_cmpd ("HPh4H");
  l_help.target_compound (l_cmpd);
  bool_test (not l_help.target_compound ().empty (),  "target compound not empty after setting.");
  bool_test (l_cmpd == l_help.target_compound (), "target compound matches set value \"" + l_cmpd + "\"");
  
  ////////////////////////////////////////
  // Test setting the experiment reference
  const std::string l_expt ("OL08-2-87");
  l_help.target_experiment (l_expt);
  bool_test (not l_help.target_experiment ().empty (),  "target experiment not empty after setting.");
  bool_test (l_expt == l_help.target_experiment (), "target experiment matches set value \"" + l_expt + "\"");
  
  ///////////////////////////////
  // Test setting the target file
  const std::string l_file ("test_me.gout");
  l_help.target_file (l_file);
  bool_test (not l_help.target_file ().empty (), "target file name not empty after setting.");
  bool_test (boost::filesystem::path (l_file) == l_help.target_file (), "target file name matches set value \"" + l_file + "\"");
  
  ////////////////////////////////////////
  // Test setting the experiment reference
  const std::string l_formula ("G120");
  l_help.target_formula (l_formula);
  bool_test (not l_help.target_experiment ().empty (),  "target formula not empty after setting.");
  bool_test (l_formula == l_help.target_formula (), "target formula matches set value \"" + l_formula + "\"");
  
  //////////////////////////////////
  // Test setting the memo reference
  const std::string l_memo ("OL08-2-87-FREQ");
  l_help.target_memo (l_memo);
  bool_test (not l_help.target_memo ().empty (), "target memo not empty after setting.");
  bool_test (l_memo == l_help.target_memo (), "target memo matches set value \"" + l_memo + "\".");
  
  //////////////////////////////////
  // Test setting the memo reference
  const std::string l_mems ("OL08-2-87-FREQ.xml");
  l_help.target_memo_src (l_mems);
  bool_test (not l_help.target_memo_src ().empty (), "target memo-src not empty after setting.");
  bool_test (l_mems == l_help.target_memo_src (), "target memo-src matches set value \"" + l_mems + "\".");
}


} // namespace supplier
