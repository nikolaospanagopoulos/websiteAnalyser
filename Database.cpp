#include "Database.hpp"
#include "JsonDownloader.hpp"

Database::Database() {
  try {

    driver = sql::mariadb::get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    con->setSchema("grapeshotClone");

    createCategoriesTable();
    createWordsTable();

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

bool Database::categoryExists(const std::string &category) {

  sql::ResultSet *res;
  sql::Statement *stmt;

  stmt = con->createStatement();

  std::string query = "SELECT * FROM categories WHERE name=";

  query.append("\"" + category + "\"");

  res = stmt->executeQuery(query);

  size_t columns = res->rowsCount();

  delete res;
  delete stmt;
  if (columns > 0) {

    return true;
  }
  return false;
}

void Database::createCategory(const std::string &category) {
  try {

    if (categoryExists(category)) {

      std::cout << "Category already exists\n";
      return;
    }

    std::string query{"INSERT IGNORE INTO categories (name) VALUES "};

    query.append("(\"" + category + "\");");

    sql::Statement *stmt;
    sql::ResultSet *res;
    stmt = con->createStatement();
    res = stmt->executeQuery(query);

    delete stmt;
    delete res;
  } catch (sql::SQLException &e) {
    std::cerr << e.what() << std::endl;
  }
}

std::string Database::getCategoryId(const std::string &categoryName) {

  sql::Statement *stmt;
  sql::ResultSet *res;

  stmt = con->createStatement();
  std::string queryToGetId = "SELECT category_id FROM categories WHERE name=";
  queryToGetId.append("\"" + categoryName + "\"");
  res = stmt->executeQuery(queryToGetId);
  sql::ResultSetMetaData *res_meta = res->getMetaData();
  int columns = res_meta->getColumnCount();
  std::string categoryId{};
  while (res->next()) {
    for (int i = 1; i <= columns; i++) {
      categoryId = res->getString(i);
    }
  }
  delete stmt;
  delete res;

  delete res_meta;
  return categoryId;
}

void Database::addWords() {

  try {

    std::string categoryName{};
    std::cout << "add a category name: \n";

    std::cin >> categoryName;

    createCategory(categoryName);

    std::cout << "What theme would you like to add ? \n";
    std::string theme{};
    std::cin >> theme;

    JsonDownloader *themeDownloader = new JsonDownloader{};

    themeDownloader->fillJsonVector(theme);

    std::vector<std::string *> *results = themeDownloader->getWordsFromJson();

    std::string categoryId = getCategoryId(categoryName);

    sql::Statement *stmt;
    sql::ResultSet *res;
    stmt = con->createStatement();
    std::string queryToAddWords =
        "INSERT IGNORE INTO words (category_id,text) VALUES ";

    for (auto *word : *results) {
      queryToAddWords.append("(" + categoryId + "," + "\"" + *word + "\"), ");
    }
    queryToAddWords.pop_back();
    queryToAddWords.pop_back();
    queryToAddWords.append(";");

    std::cout << queryToAddWords << std::endl;
    res = stmt->executeQuery(queryToAddWords);
    for (auto ptr : *results) {
      delete ptr;
    }
    delete results;
    delete themeDownloader;
    delete res;
    delete stmt;
  } catch (sql::SQLException &e) {
    std::cerr << e.what() << std::endl;
  }
}
