#include "Database.hpp"

Database::Database() {
  try {

    driver = sql::mariadb::get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    con->setSchema("grapeshotClone");

    createCategoriesTable();
    createWordsTable();

    std::cout << "wfefefe\n";
  } catch (sql::SQLException &e) {
    std::cout << "# ERR: " << e.what() << std::endl;
  }
}

Database::~Database() { delete con; }

void Database::createCategoriesTable() {

  sql::Statement *stmt;
  sql::ResultSet *res;
  stmt = con->createStatement();
  std::string query{

      "CREATE TABLE IF NOT EXISTS categories (category_id INT NOT NULL "
      "AUTO_INCREMENT, name VARCHAR(50) NOT NULL, PRIMARY KEY(category_id))"};

  try {
    res = stmt->executeQuery(query);
    delete stmt;
    delete res;
  } catch (sql::SQLException &e) {
    std::cout << "nikos";
    std::cout << "# ERR: " << e.what() << std::endl;
  }
}
void Database::createWordsTable() {
  sql::Statement *stmt;
  sql::ResultSet *res;

  stmt = con->createStatement();
  std::string query{
      "CREATE TABLE IF NOT EXISTS words ( word_id INT  NOT NULL "
      "AUTO_INCREMENT, text "
      "VARCHAR(250) NOT NULL, category_id INT, FOREIGN KEY (category_id) "
      "REFERENCES categories (category_id), PRIMARY KEY (word_id))"};
  try {
    res = stmt->executeQuery(query);
    delete stmt;
    delete res;
  } catch (sql::SQLException &e) {
    std::cout << "# ERR: " << e.what() << std::endl;
  }
}
