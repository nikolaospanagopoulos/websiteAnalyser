#include "JsonDownloader.hpp"
#include "CustomException.hpp"
#include "helpers.h"
#include <curl/curl.h>

JsonDownloader::JsonDownloader() {

  response = new std::string{};

  jsonObjects = new std::vector<json *>{};
}

JsonDownloader::~JsonDownloader() {
  delete response;
  response = nullptr;
  for (auto obj : *jsonObjects) {
    delete obj;
  }
  delete jsonObjects;
  jsonObjects = nullptr;
}

std::string JsonDownloader::buildRequestUrl(const std::string &word) const {

  std::string firstApiPart{
      "https://api.wordassociations.net/associations/v1.0/json/"
      "search?apikey=044e5f39-d712-40ee-982c-5af57d6d3245&text="};

  std::string lookFor = word;

  std::string restOfApiUrl{"&lang=en"};

  return firstApiPart + lookFor + restOfApiUrl;
};

json *JsonDownloader::sendRequest(const std::string &word) {

  std::string urlForRequest = buildRequestUrl(word);

  CURL *curl{};
  CURLcode res{};

  std::string s;

  curl = curl_easy_init();

  if (curl) {

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackToWrite);
    curl_easy_setopt(curl, CURLOPT_URL, urlForRequest.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      throw CustomException((char *)(curl_easy_strerror(res)));
    }
  }
  curl_easy_cleanup(curl);
  curl_global_cleanup();

  json *jsonResponse = new json{};
  *jsonResponse = json::parse(*response);

  response->clear();

  return jsonResponse;
  ;
}

void JsonDownloader::fillJsonVector(const std::string &word) {

  jsonObjects->push_back(sendRequest(word));
};

std::vector<json *> *JsonDownloader::getJsonResponses() const {

  return jsonObjects;
}

std::vector<std::string *> *JsonDownloader::getWordsFromJson() const {

  std::vector<std::string *> *wordsVec = new std::vector<std::string *>{};

  for (auto *obj : *jsonObjects) {

    auto jsonData = obj->at("response")[0]["items"];
    size_t jsonDataSize = jsonData.size();
    for (size_t i{}; i < jsonDataSize; i++) {
      std::string *result = new std::string{jsonData[i]["item"]};
      wordsVec->push_back(result);
    }
  }

  return wordsVec;
}
