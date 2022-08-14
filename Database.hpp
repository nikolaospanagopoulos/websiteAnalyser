#pragma once
#include <iostream>
#include <mariadb/conncpp.hpp>

class Database {

private:
  sql::Driver *driver;
  sql::Connection *con;

  bool categoryExists(const std::string &category);
  void createCategoriesTable();
  void createWordsTable();
  std::string getCategoryId(const std::string &categoryName);

public:
  std::vector<std::string *> *
  getResults(std::vector<std::string *> *resultsNumVec);
  std::vector<std::string *> *
  analyzeResults(const std::vector<std::string *> *resultsWords);
  void showTables();
  void createCategory(const std::string &category);
  void addWords();
  void showWordsByCategory();
  void deleteCategory();
  Database();
  ~Database();
};
