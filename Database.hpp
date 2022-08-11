#pragma once

#include <iostream>
#include <mariadb/conncpp.hpp>
class Database {

private:
  sql::Driver *driver;
  sql::Connection *con;

  void createCategoriesTable();
  void createWordsTable();

public:
  Database();
  ~Database();
};
