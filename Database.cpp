#include "Database.hpp"
#include "CustomException.hpp"
#include "JsonDownloader.hpp"
#include <map>

Database::Database() {

  driver = sql::mariadb::get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306", "root", "");
  std::cout << con;
  if (!con) {
    throw CustomException((char *)("couldnt connect to db"));
  }
  con->setSchema("grapeshotClone");

  createCategoriesTable();
  createWordsTable();
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
    throw CustomException((char *)(e.what()));
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
      "REFERENCES categories (category_id) ON DELETE CASCADE, PRIMARY KEY "
      "(word_id))"};
  try {
    res = stmt->executeQuery(query);
    delete stmt;
    delete res;
  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
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
    throw CustomException((char *)(e.what()));
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
    throw CustomException((char *)(e.what()));
  }
}

void Database::showTables() {

  try {

    std::cout << "Select a table name: ";
    std::string tableName = {};
    std::cin >> tableName;
    sql::Statement *stmt;
    sql::ResultSet *res;
    stmt = con->createStatement();
    std::string whatToSelect = tableName == "words"        ? "text"
                               : tableName == "categories" ? "name"
                                                           : "*";

    std::string query{"SELECT "};

    query.append(whatToSelect);
    query += " FROM ";

    query.append(tableName);
    res = stmt->executeQuery(query);
    sql::ResultSetMetaData *res_meta = res->getMetaData();

    size_t columns = res_meta->getColumnCount();

    std::cout << query << std::endl;

    while (res->next()) {
      for (size_t i = 1; i <= columns; i++) {
        std::cout << "| " << res->getString(i) << " |" << std::endl;
      }
    }
    delete stmt;
    delete res;
    delete res_meta;
  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
}
void Database::showWordsByCategory() {

  try {

    std::cout << "Select a category name: ";

    std::string categoryName{};

    std::cin >> categoryName;

    std::string categoryId = getCategoryId(categoryName);

    if (categoryId.empty()) {
      std::cout << "cannot find category" << std::endl;
      return;
    }

    sql::Statement *stmt;
    stmt = con->createStatement();
    sql::ResultSet *res;

    std::string query = "SELECT text FROM words WHERE category_id=";
    query.append(categoryId);

    res = stmt->executeQuery(query);
    sql::ResultSetMetaData *res_meta = res->getMetaData();

    size_t columns = res_meta->getColumnCount();
    while (res->next()) {
      for (size_t i = 1; i <= columns; i++) {
        std::cout << "| " << res->getString(i) << " |" << std::endl;
      }
    }
    delete res_meta;
    delete res;
    delete stmt;
  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
}
std::vector<std::string *> *
Database::analyzeResults(const std::vector<std::string *> *resultsWords) {
  try {

    if (resultsWords->size() == 0) {

      throw CustomException((char *)"no results");
    }

    std::string statement = {"SELECT category_id FROM words WHERE "};

    for (auto *word : *resultsWords) {

      std::string text = "text LIKE ";

      std::string orSql = " OR ";
      statement.append(text + "\"" + *word + "\"" + orSql);
    }

    auto wordToRemove = statement.find_last_of("OR");

    statement.erase(wordToRemove - 1);

    statement.append(";");

    sql::Statement *stmt;
    stmt = con->createStatement();

    sql::ResultSet *res;
    res = stmt->executeQuery(statement);
    sql::ResultSetMetaData *res_meta = res->getMetaData();

    size_t columns = res_meta->getColumnCount();
    if (columns == 0) {
      throw CustomException(const_cast<char *>("no results in db"));
    }
    std::vector<std::string *> *categoryIdsVector =
        new std::vector<std::string *>{};
    while (res->next()) {
      for (size_t i = 1; i <= columns; i++) {

        categoryIdsVector->push_back(new std::string{res->getString(i)});
      }
    }

    delete res_meta;
    delete res;
    delete stmt;

    return categoryIdsVector;
  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
  return nullptr;
}

std::string Database::getResults(std::vector<std::string *> *resultsNumVec) {

  std::string statement = {"SELECT name FROM categories WHERE "};

  for (auto *id : *resultsNumVec) {

    std::string text = "category_id LIKE ";

    std::string orSql = " OR ";
    statement.append(text + "\"" + *id + "\"" + orSql);
  }

  auto wordToRemove = statement.find_last_of("OR");

  statement.erase(wordToRemove - 1);

  statement.append(";");

  sql::Statement *stmt;
  stmt = con->createStatement();
  sql::ResultSet *res;

  res = stmt->executeQuery(statement);
  sql::ResultSetMetaData *res_meta = res->getMetaData();

  size_t columns = res_meta->getColumnCount();
  while (res->next()) {
    for (size_t i = 1; i <= columns; i++) {

      std::cout << res->getString(i) << std::endl;
    }
  }
  delete stmt;
  delete res;
  delete res_meta;
  return statement;
}

void Database::deleteCategory() {
  try {

    sql::Statement *stmt;
    stmt = con->createStatement();
    sql::ResultSet *res;

    std::cout << "select a category to delete: \n";

    std::string categoryName{};

    std::cin >> categoryName;

    std::string categoryId = getCategoryId(categoryName);

    if (categoryId.size() == 0) {
      throw CustomException((char *)("category doesnt exist"));
    }

    std::string statement = {"DELETE FROM categories where category_id="};

    statement.append(categoryId);

    std::cout << statement;
    res = stmt->executeQuery(statement);

    delete res;
    delete stmt;
  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
}
