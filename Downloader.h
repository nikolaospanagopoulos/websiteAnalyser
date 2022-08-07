#pragma once
#include <iostream>

class Downloader {

private:
  bool validateUrl(const std::string url);

public:
  std::string *requestData(std::string websiteUrl);
};
