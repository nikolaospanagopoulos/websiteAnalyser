#pragma once
#include "json.h"
#include <iostream>
#include <mariadb/conncpp.hpp>

using json = nlohmann::json;
class Database {

private:
  sql::Driver *driver;
  sql::Connection *con;

  bool categoryExists(const std::string &category);
  void createCategoriesTable();
  void createWordsTable();
  void createDatabase();
  std::string getCategoryId(const std::string &categoryName);

public:
  std::vector<std::string *> *
  getResults(std::vector<std::string *> *resultsNumVec);
  std::vector<std::string *> *
  analyzeResults(const std::vector<std::string *> *resultsWords);
  void showTables();
  void createCategory(const std::string &category);
  json addWords(std::string &categoryName, std::string &theme);
  void showWordsByCategory();
  void deleteCategory();

  Database();
  ~Database();
};
