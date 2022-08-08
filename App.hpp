#pragma once
#include <string>

class App {

  std::string website{};

public:
  App(std::string url) : website(url) {}
  std::string *prepareHtmlData();
  ~App() {}
};
