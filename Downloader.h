#pragma once
#include <iostream>

class Downloader {

private:
  std::string *response{};
  bool validateUrl(const std::string url);

public:
  std::string *requestData(std::string websiteUrl);
  Downloader();
  ~Downloader();
};
