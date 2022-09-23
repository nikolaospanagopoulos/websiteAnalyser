#include "Database.hpp"
#include "CustomException.hpp"
#include "JsonDownloader.hpp"
#include "JsonResponse.h"
#include <cmath>
#include <map>

using json = nlohmann::json;
Database::Database() {
sql::ConnectOptionsMap connection_properties;

connection_properties["hostName"] = "db";
connection_properties["userName"] = "root";
connection_properties["password"] = "pass";
connection_properties["port"] = "3306";
 


  driver = sql::mariadb::get_driver_instance();



  con = driver->connect(connection_properties);
  if (!con) {
    throw CustomException((char *)("couldnt connect to db"));
  }
  createDatabase();
  con->setSchema("grapeshotClone");

  createCategoriesTable();
  createWordsTable();
  createWebsiteTable();
  createPercentagesTable();
}

void Database::createDatabase() {

  try {

    sql::Statement *stmt;
    sql::ResultSet *res;
    std::string query{"CREATE DATABASE IF NOT EXISTS grapeshotClone;"};
    stmt = con->createStatement();
    res = stmt->executeQuery(query);
    delete res;
    delete stmt;
  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
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
    throw CustomException((char *)(e.what()));
  }
}
void Database::createWebsiteTable() {

  sql::Statement *stmt;
  sql::ResultSet *res;
  stmt = con->createStatement();
  std::string query{

      "CREATE TABLE IF NOT EXISTS websites (website_id INT NOT NULL "
      "AUTO_INCREMENT, url VARCHAR(550) NOT NULL,UNIQUE(url), PRIMARY "
      "KEY(website_id))"};

  try {
    res = stmt->executeQuery(query);
    delete stmt;
    delete res;
  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
}
void Database::createPercentagesTable() {
  sql::Statement *stmt;
  sql::ResultSet *res;

  stmt = con->createStatement();
  std::string query{
      "CREATE TABLE IF NOT EXISTS percentages ( percentage_id INT  NOT NULL "
      "AUTO_INCREMENT, theme "
      "VARCHAR(250) NOT NULL, percentage DOUBLE NOT NULL, website_id INT, "
      "FOREIGN KEY "
      "(website_id) "
      "REFERENCES websites (website_id) ON DELETE CASCADE, PRIMARY KEY "
      "(percentage_id))"};
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
      "VARCHAR(250) NOT NULL, category_id INT,  UNIQUE (text), FOREIGN KEY "
      "(category_id) "
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

json Database::addWords(std::string &categoryName, std::string &theme) {

  try {

    if (categoryName.empty() || theme.empty()) {
      throw CustomException((char *)"one of the fields is empty");
    }

    createCategory(categoryName);

    JsonDownloader *themeDownloader = new JsonDownloader{};

    themeDownloader->fillJsonVector(theme, "50");

    std::vector<std::string> *results = themeDownloader->getWordsFromJson();

    std::string categoryId = getCategoryId(categoryName);

    sql::Statement *stmt;
    sql::ResultSet *res;
    stmt = con->createStatement();
    std::string queryToAddWords =
        "INSERT IGNORE INTO words (category_id,text) VALUES ";

    for (auto &word : *results) {
      queryToAddWords.append("(" + categoryId + "," + "\"" + word + "\"), ");
    }
    queryToAddWords.pop_back();
    queryToAddWords.pop_back();
    queryToAddWords.append(";");

    std::cout << queryToAddWords << std::endl;
    res = stmt->executeQuery(queryToAddWords);

    // prepare json object to return;

    json addWordsResponse = json{};

    (addWordsResponse)["status"] = "ok";

    (addWordsResponse)["wordsAdded"] = *results;

    (addWordsResponse)["query"] = queryToAddWords;

    delete results;
    delete themeDownloader;
    delete res;
    delete stmt;

    return addWordsResponse;

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
    if (!res->next()) {

      throw CustomException((char *)("table is empty"));
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
      throw CustomException((char *)("category doesnt exist"));
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
std::map<std::string, size_t> *
Database::analyzeResults(const std::vector<std::string> *resultsWords) {
  try {

    if (resultsWords->size() == 0) {

      throw CustomException((char *)"no results");
    }

    std::string statement = {"SELECT category_id FROM words WHERE "};

    for (auto word : *resultsWords) {

      std::string text = "text = ";

      std::string orSql = " OR ";
      statement.append(text + "\"" + word + "\"" + orSql);
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
    std::map<std::string, size_t> *resultsMap =
        new std::map<std::string, size_t>{};
    while (res->next()) {
      for (size_t i = 1; i <= columns; i++) {

        std::cout << "category " << (std::string)res->getString(i) << std::endl;
        (*resultsMap)[(std::string)(res->getString(i))]++;
      }
    }

    delete res_meta;
    delete res;
    delete stmt;

    return resultsMap;
  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
  return nullptr;
}

std::string Database::getCategoryName(const std::string &categoryId) {

  std::string statement = {"SELECT name FROM categories WHERE "};

  std::string text = "category_id = ";
  statement.append(text + "\"" + categoryId + "\"");

  statement.append(";");

  sql::Statement *stmt;
  stmt = con->createStatement();
  sql::ResultSet *res;

  res = stmt->executeQuery(statement);
  sql::ResultSetMetaData *res_meta = res->getMetaData();

  size_t columns = res_meta->getColumnCount();
  std::string categoryName{};
  while (res->next()) {
    for (size_t i = 1; i <= columns; i++) {

      categoryName = (std::string(res->getString(i)));
      std::cout << categoryName << std::endl;
    }
  }

  delete stmt;
  delete res;
  delete res_meta;
  return categoryName;
}

std::vector<std::string *> *
Database::getResults(std::map<std::string, size_t> *resultsMap) {

  std::vector<std::string *> *results = new std::vector<std::string *>{};
  if (resultsMap->empty()) {
    results->push_back(new std::string{"no results"});
    return results;
  }

  for (auto pair : *resultsMap) {
    std::cout << pair.first << pair.second << std::endl;
    results->push_back(new std::string(getCategoryName(pair.first)));
  }

  return results;
}

std::map<std::string, double> *
Database::calculatePercentage(std::map<std::string, size_t> *resultsMap) {

  int result{};

  for (auto &pair : *resultsMap) {
    result += pair.second;
  }
  std::cout << "result: " << result << std::endl;

  std::map<std::string, double> *percentageMap =
      new std::map<std::string, double>{};
  for (auto &pair : *resultsMap) {

    double decimalVal = ((double)pair.second / result) * 100;
    double cutDecimal = std::ceil(decimalVal * 100) / 100.0;

    percentageMap->insert(std::pair<std::string, double>(

        getCategoryName(pair.first), cutDecimal));
  }

  for (auto &pair : *percentageMap) {
    std::cout << "first: " << pair.first << " second: " << pair.second
              << std::endl;
  }

  return percentageMap;
}

void Database::deleteCategory(const std::string &categoryName) {
  try {

    sql::Statement *stmt;
    stmt = con->createStatement();
    sql::ResultSet *res;

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
void Database::insertWebsiteToDb(
    const std::string &website, const std::map<std::string, double> *resultsMap,
    const std::vector<std::string *> *results) {
  if (resultsMap->empty() || results->empty()) {
    return;
  }

  try {

    std::string query{"INSERT IGNORE INTO websites (url) VALUES  "};

    query.append("(\"" + website + "\")");

    sql::Statement *stmt;
    stmt = con->createStatement();
    sql::ResultSet *res;
    res = stmt->executeQuery(query);

    delete res;
    delete stmt;

    insertPercentagesToDb(website, resultsMap, results);

  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
}

void Database::insertPercentagesToDb(
    const std::string &website, const std::map<std::string, double> *resultsMap,
    const std::vector<std::string *> *results) {

  try {

    int websiteID = getWebsiteId(website);

    for (const auto &mypair : *resultsMap) {
      insertMapPairToDb(mypair, websiteID);
    }

  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
}

void Database::insertMapPairToDb(const std::pair<std::string, double> &mapPair,
                                 int &websiteID) {

  try {
    sql::Statement *stmt;
    stmt = con->createStatement();
    sql::ResultSet *res;
    std::string query{
        "INSERT INTO percentages (theme,percentage,website_id) VALUES "};
    query.append("(\"" + mapPair.first + "\"" + "," +
                 std::to_string(mapPair.second) + "," +
                 std::to_string(websiteID) + ")");

    res = stmt->executeQuery(query);

    delete stmt;
    delete res;

  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
}

int Database::getWebsiteId(const std::string &website) {

  try {

    std::string query{"SELECT website_id FROM websites WHERE url="};

    query.append("\"" + website + "\"");

    sql::Statement *stmt;
    stmt = con->createStatement();
    sql::ResultSet *res;
    res = stmt->executeQuery(query);

    int website_id{-1};
    if (res->next()) {

      website_id = res->getInt("website_id");
    }

    delete res;
    delete stmt;

    return website_id;

  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
}

json Database::getDbResponse(const std::string &website)

{

  try {
    sql::Statement *stmt;
    stmt = con->createStatement();
    sql::ResultSet *res;
    int website_id = getWebsiteId(website);

    std::string query{"SELECT * FROM percentages WHERE website_id="};
    query.append(std::to_string(website_id));
    res = stmt->executeQuery(query);

    sql::ResultSetMetaData *res_meta = res->getMetaData();

    size_t columns = res_meta->getColumnCount();

    std::vector<std::string *> *results = new std::vector<std::string *>{};

    std::map<std::string, double> *cachedResultsMap =
        new std::map<std::string, double>{};

    while (res->next()) {
      results->push_back(new std::string(res->getString("theme")));
      cachedResultsMap->insert(std::pair<std::string, double>{
          (std::string)res->getString("theme"),
          res->getDouble("percentage"),
      });
    }

    json finalResult = createJsonRespone(results, cachedResultsMap);

    delete res;
    delete stmt;
    delete res_meta;

    for (auto &ptr : *results) {
      delete ptr;
    }

    delete results;

    delete cachedResultsMap;

    return finalResult;

  } catch (sql::SQLException &e) {
    throw CustomException((char *)(e.what()));
  }
}
