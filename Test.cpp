// Includes
#include <iostream>
#include <mariadb/conncpp.hpp>
using namespace std;
#include <mariadb/conncpp/Driver.hpp>
#include <mariadb/conncpp/Exception.hpp>
#include <mariadb/conncpp/ResultSet.hpp>
#include <mariadb/conncpp/SQLString.hpp>
#include <mariadb/conncpp/Statement.hpp>

int main(void) {
  cout << endl;
  cout << "Running 'SELECT 'Hello World!' << AS _message'..." << endl;

  try {
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    /* Create a connection */
    driver = sql::mariadb::get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("nikos");

    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
    while (res->next()) {
      cout << "\t... MySQL replies: ";
      /* Access column data by alias or column name */
      cout << res->getString("_message") << endl;
      cout << "\t... MySQL says it again: ";
      /* Access column data by numeric offset, 1 is the first column */
      cout << res->getString(1) << endl;
    }
    delete res;
    delete stmt;
    delete con;

  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }

  cout << endl;

  return EXIT_SUCCESS;
}
