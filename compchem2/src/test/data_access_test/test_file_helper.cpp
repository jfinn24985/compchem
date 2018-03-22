// ------------------------------------------------------------
// class: test_file_helper
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/data_access_test/test_file_helper.hpp"
#include "data_access/names.hpp"
#include "data_access/persistence.hpp"
#include "data_access/xml_access_helper.hpp"

namespace data_access {

/**
 * The singleton
 */
test_file_helper test_file_helper::exemplar;

/**
 * Override in derived classes to perform unit test.
 */
void test_file_helper::main_test() const 
{
  xml_access_helper & l_helper (xml_access_helper::helper ());
  if (not l_helper.empty ())
  {
    // Print list of search directories
    int l_c (1);
    for (xml_access_helper::const_iterator l_i (l_helper.begin ()); l_i != l_helper.end (); ++l_i, ++l_c)
    {
      log () << WTCH << "Directory [" << l_c << "] = \"" << l_i->string () << "\"\n";
    }
  }
  log () << WTCH << "Default public ID: " << l_helper.default_public_id () << "\n";
  log () << WTCH << "Default system ID: " << l_helper.default_system_id () << "\n";
  log () << WTCH << "Set public ID: " << l_helper.public_id () << "\n";
  log () << WTCH << "Set system ID: " << l_helper.system_id () << "\n";
  bool_test (l_helper.default_public_id () == l_helper.public_id (), "default public ID equals set value.");
  bool_test (l_helper.default_system_id () == l_helper.system_id (), "default system ID equals set value.");
  log () << WTCH << "Auto-fix is : " << (l_helper.auto_fix ()?"yes":"no") << "\n";

}

test_file_helper::~test_file_helper() 
{}


} // namespace data_access
