// ------------------------------------------------------------
// class: test_meta_datum
// $revision:$
// $log:$
// ------------------------------------------------------------

/**
 * Test meta_datum
 */
#include "test/data_access_test/test_meta_datum.hpp"
#include <boost/bind.hpp>

#include "data_access/meta_datum.hpp"

namespace data_access {

/**
 * Class exemplar
 */
test_meta_datum test_meta_datum::exemplar;

void test_meta_datum::main_test() const 
{
  // LOCAL
  std::auto_ptr<meta_datum> l_test;
  std::string l_name (names::compound_name ());
  std::string l_names (names::project_name ());
  std::string l_keyattr (names::label_name ());
  std::string l_attrs (names::formula_name ());
  
  /////////////////
  // Test 1: datum with child, key and attribute
  log () << TTLE << "Test 1: meta_datum with child, key and attribute\n";
  l_test.reset (new meta_datum (l_name, l_keyattr, l_names, l_attrs));
  // Test basic attributes
  bool_test (l_test->has_attributes (), "has some attributes");
  bool_test (not l_test->empty_children (), "has valid child nodes");
  bool_test (l_test->name == "compound", "matched tag-name");
  bool_test (l_test->key_attribute == "label", "matched key attribute");
  // Test if child tag is found by value.
  bool_test (l_test->has_child_tag ("project")
             , "found child tag from value");
  // Test if child tag is found by value.
  bool_test (not l_test->has_child_tag ("formula")
             , "did not find unknown child tag from value");
  // Test if attribute name is found by value.
  bool_test (l_test->has_attr_name ("formula")
             , "found attribute name from value");
  // Test if attribute name is found by value.
  bool_test (not l_test->has_attr_name ("compound")
             , "did not find unknown attribute name from value");
  // Test if an attribute with the given value is valid
  bool_test (l_test->valid_attribute ("label", "a value")
             , "accepted valid attribute name");
  bool_test (l_test->valid_attribute ("formula", "a value")
             , "accepted valid attribute name");
  bool_test (not l_test->valid_attribute ("compound", "a value")
             , "rejected invalid attribute name");
  // Test static search function and public attributes
  bool_test ("compound" == meta_datum::by_name ("compound").name
             , "found object by name and matched name");
  bool_test ("label" == meta_datum::by_name ("compound").key_attribute
             , "found object by name and matched key attribute");
  // Test attribute search
  unsigned int l_attr_count (0);
  for (meta_datum::const_iterator l_i = l_test->begin_attr (); l_i != l_test->end_attr (); ++l_i)
  {
    bool_test (*l_i == "label" or *l_i == "formula", "matched attribute name to list");
    ++l_attr_count;
  }
  bool_test (2U == l_attr_count, "attribute list has two values");
  
  /////////////////////////////////
  // Test 2 meta_datum with name and key attr only
  log () << TTLE << "Test 2: meta_datum with key attribute only\n";
  l_test.reset ();
  l_test.reset (new meta_datum (l_name, l_keyattr, std::string (), std::string ()));
  // Test basic attributes
  bool_test (l_test->has_attributes (), "has some attributes");
  bool_test (l_test->empty_children (), "has no valid child nodes");
  bool_test (l_test->name == "compound", "matched tag-name");
  bool_test (l_test->key_attribute == "label", "matched key attribute");
  // Test if child tag is found by value.
  bool_test (not l_test->has_child_tag ("project"), "did not find unknown child tag from value");
  // Test if attribute name is found by value.
  bool_test (l_test->has_attr_name ("label"), "found attribute name from value");
  // Test if attribute name is found by value.
  bool_test (not l_test->has_attr_name ("compound"), "did not find unknown attribute name from value");
  // Test if an attribute with the given value is valid
  bool_test (l_test->valid_attribute ("label", "a value"), "accepted valid attribute name");
  bool_test (not l_test->valid_attribute ("compound", "a value"), "rejected invalid attribute name");
  // Test static search function and public attributes
  bool_test ("compound" == meta_datum::by_name ("compound").name
             , "found object by name and matched name");
  bool_test ("label" == meta_datum::by_name ("compound").key_attribute
             , "found object by name and matched key attribute");
  // Test attribute search
  l_attr_count = 0;
  for (meta_datum::const_iterator l_i = l_test->begin_attr (); l_i != l_test->end_attr (); ++l_i)
  {
    bool_test (*l_i == "label", "matched attribute name to list");
    ++l_attr_count;
  }
  bool_test (1U == l_attr_count, "attribute list has one value");
  
  /////////////////////////////////
  // Test 3 meta_datum with name and no-key attr
  log () << TTLE << "Test 3: meta_datum with name only\n";
  l_test.reset ();
  l_test.reset (new meta_datum (l_name, std::string (), std::string (), std::string ()));
  // Test basic attributes
  bool_test (not l_test->has_attributes (), "has no attributes");
  bool_test (l_test->empty_children (), "has no valid child nodes");
  bool_test (l_test->name == "compound", "matched tag-name");
  bool_test (l_test->key_attribute.empty (), "empty key attribute");
  // Test if child tag is found by value.
  bool_test (not l_test->has_child_tag ("project"), "did not find unknown child tag from value");
  // Test if attribute name is found by value.
  bool_test (not l_test->has_attr_name ("label"), "did not find unknown attribute name from value");
  // Test if attribute name is found by value.
  bool_test (not l_test->has_attr_name ("compound"), "did not find unknown attribute name from value");
  // Test if an attribute with the given value is valid
  bool_test (not l_test->valid_attribute ("compound", "a value"), "rejected invalid attribute name");
  // Test static search function and public attributes
  bool_test ("compound" == meta_datum::by_name ("compound").name
             , "found object by name and matched name");
  // Test attribute search
  l_attr_count = 0;
  for (meta_datum::const_iterator l_i = l_test->begin_attr (); l_i != l_test->end_attr (); ++l_i)
  {
    bool_test (l_i->empty (), "attribute is empty");
    ++l_attr_count;
  }
  bool_test (0U == l_attr_count, "attribute list empty");

}


} // namespace data_access
