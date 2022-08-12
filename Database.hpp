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
  void createCategory(const std::string &category);
  void addWords();
  Database();
  ~Database();
};
