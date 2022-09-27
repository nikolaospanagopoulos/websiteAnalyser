#pragma once
#include "json.h"
#include <iostream>
#include <mariadb/conncpp.hpp>
#include <mariadb/conncpp/DriverManager.hpp>
using json = nlohmann::json;
class Database {

private:
  sql::Driver *driver;
  sql::Connection *con;

  std::string getCategoryName(const std::string &categoryId);
  bool categoryExists(const std::string &category);
  void createCategoriesTable();
  void createWordsTable();
  void createDatabase();
  std::string getCategoryId(const std::string &categoryName);
  void createWebsiteTable();
  void createPercentagesTable();

  void insertMapPairToDb(const std::pair<std::string, double> &mapPair,
                         int &websiteID);
  void insertPercentagesToDb(const std::string &website,
                             const std::map<std::string, double> *resultsMap,
                             const std::vector<std::string *> *results);

public:
void connectDB();
  int getWebsiteId(const std::string &website);
  json getDbResponse(const std::string &website);
  void insertWebsiteToDb(const std::string &website,
                         const std::map<std::string, double> *resultsMap,
                         const std::vector<std::string *> *results);
  bool checkIfWebsiteExistsInDb(const std::string &website);
  std::map<std::string, double> *
  calculatePercentage(std::map<std::string, size_t> *resultsMap);
  std::vector<std::string *> *
  getResults(std::map<std::string, size_t> *resultsMap);
  std::map<std::string, size_t> *
  analyzeResults(const std::vector<std::string> *resultsWords);
  void showTables();
  void createCategory(const std::string &category);
  json addWords(std::string &categoryName, std::string &theme);
  void showWordsByCategory();
  void deleteCategory(const std::string &categoryName);

  Database();
  ~Database();
};
