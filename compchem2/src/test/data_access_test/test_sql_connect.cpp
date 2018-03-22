// ------------------------------------------------------------
// class: test_sql_connect
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "test/data_access_test/test_sql_connect.hpp"

#include <mysql++/mysql++.h>
namespace data_access {

test_sql_connect::db_user::db_user(const test_sql_connect::db_user & source)
: username (source.get_username ())
, password (source.get_password ())
{
}

/**
 * Class helper that queries a user through stdin/stdout.
 */
std::string test_sql_connect::db_user::s_query_function(const std::string& a_query)
{
  std::string Result;
  std:: cout << a_query << " = ";
  std::cin >> Result;
  return Result;
}

/**
 * Exemplar of test
 */
test_sql_connect test_sql_connect::exemplar(std::string("test"));

/**
 * Main test method
 */
int test_sql_connect::main_test(int argc, char** argv) const {
  // LOCAL
  // Create local locator
  db_locator l_url(*url);
  db_user l_user, l_baduser;
  // PRECONDITION
  // REQUIRE()
  // DO
  std::cout << "###### Initialise #####" << std::endl;
  if (false == l_user.is_valid ())
  {
  	l_user.query_user ();
  }
  ALWAYS(l_user.is_valid (), "Require  non-empty values for username and password");
  
  std::cout << "###### Test 1: Open database with valid information #####" << std::endl;
  std::cout << "###### Please run this test-suite at least once while #####" << std::endl;
  std::cout << "###### MySQL server is not running. #####" << std::endl;
  connection_test (l_url, l_user);
  std::cout << std::endl << std::endl;
  
  std::cout << "###### Test 2: Open database with invalid username information #####" << std::endl;
  l_baduser = l_user;
  l_baduser.set_username ("justin");
  connection_test (l_url, l_baduser);
  std::cout << std::endl << std::endl;
  
  std::cout << "###### Test 3: Open database with invalid password information #####" << std::endl;
  l_baduser = l_user;
  l_baduser.set_password ("justin");
  connection_test (l_url, l_user);
  std::cout << std::endl << std::endl;
  
  std::cout << "###### Test 4: Open database with invalid host information #####" << std::endl;
  l_url.set_host ("justinf4");
  connection_test (l_url, l_user);
  std::cout << std::endl << std::endl;
  
  std::cout << "###### Test 5: Open database with non-local host information #####" << std::endl;
  l_url.set_host ("justinf5");
  connection_test (l_url, l_user);
  std::cout << std::endl << std::endl;
  
  std::cout << "###### Test 6: Open database with non-local host and default port information #####" << std::endl;
  l_url.set_port (0);
  connection_test (l_url, l_user);
  std::cout << std::endl << std::endl;
  
  std::cout << "###### Test 7: Open database with non-local host and invalid port information #####" << std::endl;
  l_url.set_port (2000);
  connection_test (l_url, l_user);
  std::cout << std::endl << std::endl;
  
  std::cout << "###### Test 8: Open database with invalid database information #####" << std::endl;
  l_url = *url;
  l_url.set_database ("justin");
  connection_test (l_url, l_user);
  std::cout << std::endl << std::endl;
  
  std::cout << "###### Test 9: Open database (local) with invalid port information #####" << std::endl;
  l_url = *url;
  l_url.set_port (2000);
  connection_test (l_url, l_user);
  std::cout << std::endl << std::endl;
  
  std::cout << "###### All tests done #####" << std::endl;
  
  // POSTCONDITION
  // ENSURE()
  return 0;
}

test_sql_connect::test_sql_connect(const std::string& a_db, const std::string& a_host, unsigned int a_port)
: unit_test()
, url(new db_locator(a_db, a_host, a_port))
 {
  // DO
  // END
}

test_sql_connect::~test_sql_connect() {
  
  if (NULL != url) { delete url; };
}

/**
 * Attempt to make a connection using a_url.
 * 
 * \pre a_user.is_valid () == true
 */
void test_sql_connect::connection_test(const test_sql_connect::db_locator & a_url, const test_sql_connect::db_user & a_user) const {
  // LOCAL
  // REQUIRE
  REQUIRE(a_user.is_valid (), "'connection_test' requires a valid db_user object");
  // DO
  try
  {
  	// Create a connection object
  	mysqlpp::Connection l_db_connect(mysqlpp::use_exceptions);
  
  	// Test connection.
  	CHECK(false == l_db_connect.connected(), "Report connected when not connected.");
  
  	// Start connection
  	l_db_connect.connect (a_url.get_database ().c_str ()
  				, a_url.get_host ().c_str ()
  				, a_user.get_username ().c_str ()
  				, a_user.get_password ().c_str ()
  				, a_url.get_port ());
  
  	// Test connection.
  	CHECK(true == l_db_connect, "Connection failed without raising an exception");
  
  	// Report connection.
  	std::cout << "Connection succeeded." << std::endl;
  	std::cout << "Database info: " << l_db_connect.info () << std::endl;
  	std::cout << "Client info  : " << l_db_connect.client_info () << std::endl;
  	std::cout << "Host info    : " << l_db_connect.host_info () << std::endl;
  	std::cout << "Server info  : " << l_db_connect.server_info () << std::endl;
  	std::cout << "Status info  : " << l_db_connect.stat () << std::endl;
  	std::cout << "API ver. info: ";
  	l_db_connect.api_version (std::cout);
  	std::cout << std::endl;
  
  
  	// Close connection.
  	l_db_connect.close ();
  
  	// Test connection.
  	CHECK(false == l_db_connect.connected(), "Report connected when not connected.");
  }
  catch (const mysqlpp::ConnectionFailed &err)
  {
  	std::cout << "[" << title() << "] Caught ConnectionFailed exception: " << err.what() << std::endl;
  }
  catch (const std::exception &err)
  {
  	std::cout << "[" << title() << "] Caught generic exception: " << err.what() << std::endl;
  };
  
}


} // namespace data_access
