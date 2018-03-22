#ifndef TEST_SQL_CONNECT_HPP
#define TEST_SQL_CONNECT_HPP

// ------------------------------------------------------------
// class: test_sql_connect
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <string>
#include "utility/unit_test.hpp"

namespace data_access {

/**
 * Test of access to a mysql database.
 */
class test_sql_connect : public utility::unit_test {
  public:
    /**
     * Information about a user who wants to connect to a db.
     */
    class db_user {
      public:
        /**
         * A function pointer to ask questions of user.
         */
        typedef std::string (*query_function)(const std::string &);


      private:
        /**
         * The username under which to access the database
         */
        std::string username;

        /**
         * DB password for user
         */
        std::string password;


      public:
        inline const std::string get_username() const;

        inline const std::string get_password() const;

        /**
         * Does this object have a non-empty user and password
         * 
         * \result true implies get_username ().empty () /= true and get_password ().empty () /= true
         */
        inline bool is_valid() const;

        /**
         * Ask a user for their username and password.
         * 
         * \pre false = is_valid ();
         * \post true = is_valid ();
         */
        inline void query_user(const query_function & a_query_func);

        /**
         * Ask a user for their username and password.
         */
        inline void query_user();

        inline void set_password(const std::string value);

        inline void set_username(const std::string value);

        inline db_user();

        inline db_user(const std::string& a_user, const std::string& a_passwd);

        inline ~db_user();

        db_user(const db_user & source);

        inline db_user & operator=(const db_user & source);

        /**
         * Class helper that queries a user through stdin/stdout.
         */
        static std::string s_query_function(const std::string& a_query);

    };
    
    /**
     * Information to connect to a database.
     */
    class db_locator {
      private:
        /**
         * The name of the database to access
         */
        std::string database;

        /**
         * The hostname where the database is located
         */
        std::string host;

        /**
         * Port to access DB on
         */
        unsigned int port;


      public:
        inline const std::string get_database() const;

        inline const std::string get_host() const;

        inline unsigned int get_port() const;

        inline db_locator(const std::string& a_db, const std::string& a_host, unsigned int a_port);

        inline ~db_locator();

        inline db_locator(const db_locator & source);

        inline db_locator & operator=(const db_locator & source);

        inline void set_database(const std::string value);

        inline void set_host(const std::string value);

        inline void set_port(const unsigned int value);

    };
    

  private:
    /**
     * Exemplar of test
     */
    static test_sql_connect exemplar;

    db_locator* url;


  public:
    /**
     * Main test method
     */
    virtual int main_test(int argc, char** argv) const;

    test_sql_connect(const std::string& a_db, const std::string& a_host=std::string("localhost"),  unsigned int a_port=3306);

    virtual ~test_sql_connect();


  private:
    test_sql_connect(const test_sql_connect & source);


  public:
    inline virtual std::string title() const;


  private:
    test_sql_connect & operator=(const test_sql_connect & source);

    /**
     * Attempt to make a connection using a_url.
     * 
     * \pre a_user.is_valid () == true
     */
    void connection_test(const db_locator & a_url, const db_user & a_user) const;

};
inline const std::string test_sql_connect::db_user::get_username() const {
  return username;
}

inline const std::string test_sql_connect::db_user::get_password() const {
  return password;
}

/**
 * Does this object have a non-empty user and password
 * 
 * \result true implies get_username ().empty () /= true and get_password ().empty () /= true
 */
inline bool test_sql_connect::db_user::is_valid() const {
  return (get_username ().empty () != true and get_password ().empty () != true);
}

/**
 * Ask a user for their username and password.
 * 
 * \pre false = is_valid ();
 * \post true = is_valid ();
 */
inline void test_sql_connect::db_user::query_user(const test_sql_connect::db_user::query_function & a_query_func) {
  // LOCAL
  // REQUIRE
  REQUIRE (is_valid () != true, "'query_user' requires that either username or password is an empty string");
  // DO
  if (get_username ().empty () == true)
  {
  	set_username ((*a_query_func)("Username"));
  
  }
  // Always query for a password
  set_password ((*a_query_func)("Password"));
  // ENSURE
  ENSURE (is_valid () == true, "'query_user' failed to set a valid username or password");
  // END
}

/**
 * Ask a user for their username and password.
 */
inline void test_sql_connect::db_user::query_user() {
  query_user (&db_user::s_query_function);
}

inline void test_sql_connect::db_user::set_password(const std::string value) {
  password = value;
}

inline void test_sql_connect::db_user::set_username(const std::string value) {
  username = value;
}

inline test_sql_connect::db_user::db_user()
: username ()
, password () {
}

inline test_sql_connect::db_user::db_user(const std::string& a_user, const std::string& a_passwd)
: username (a_user)
, password (a_passwd)
 {
}

inline test_sql_connect::db_user::~db_user() {
}

inline test_sql_connect::db_user & test_sql_connect::db_user::operator=(const test_sql_connect::db_user & source) {
  username = source.get_username ();
  password = source.get_password ();
  return *this;
}

inline const std::string test_sql_connect::db_locator::get_database() const {
  return database;
}

inline const std::string test_sql_connect::db_locator::get_host() const {
  return host;
}

inline unsigned int test_sql_connect::db_locator::get_port() const {
  return port;
}

inline test_sql_connect::db_locator::db_locator(const std::string& a_db, const std::string& a_host, unsigned int a_port)
: database(a_db)
, host(a_host)
, port(a_port) {
}

inline test_sql_connect::db_locator::~db_locator() {
}

inline test_sql_connect::db_locator::db_locator(const test_sql_connect::db_locator & source)
: database(source.get_database ())
, host(source.get_host ())
, port(source.get_port ())
 {
}

inline test_sql_connect::db_locator & test_sql_connect::db_locator::operator=(const test_sql_connect::db_locator & source) {
  database = source.get_database ();
  host = source.get_host ();
  port = source.get_port ();
  return *this;
}

inline void test_sql_connect::db_locator::set_database(const std::string value) {
  database = value;
}

inline void test_sql_connect::db_locator::set_host(const std::string value) {
  host = value;
}

inline void test_sql_connect::db_locator::set_port(const unsigned int value) {
  port = value;
}

inline std::string test_sql_connect::title() const {
  return std::string("MySQL++ library trial"); }


} // namespace data_access
#endif
