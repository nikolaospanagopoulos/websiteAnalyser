#pragma once
#include "json.h"
#include <string>
#include <vector>
using json = nlohmann::json;
class JsonDownloader {

private:
  std::string *response{};
  std::string buildRequestUrl(const std::string &word,
                              std::string wordsNumber) const;
  std::vector<json *> *jsonObjects{};

public:
  JsonDownloader();
  ~JsonDownloader();

  json *sendRequest(const std::string &word, std::string wordsNumber);
  void fillJsonVector(const std::string &word, std::string wordsNumber = "10");
  std::vector<json *> *getJsonResponses() const;
  std::vector<std::string> *getWordsFromJson() const;
};
