#include "Downloader.hpp"
#include "CustomException.hpp"
#include <cstddef>
#include <curl/curl.h>
#include <regex>
#include <string>

bool Downloader::validateUrl(const std::string url) {
  std::regex url_regex(
      "((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//"
      "=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");

  if (std::regex_match(url.c_str(), url_regex)) {
    return true;
  } else {
    return false;
  }
}

std::string answer = {};

size_t writeResponseToString(char *contents, size_t size, size_t nmemb,
                             void *userData) {
  size_t newLength = size * nmemb;
  try {

    answer.append((char *)contents, newLength);

  } catch (std::bad_alloc &e) {
    std::cerr << e.what() << std::endl;
  }

  return newLength;
}

size_t (*callBackToWrite)(char *, size_t, size_t,
                          void *) = writeResponseToString;

std::string *Downloader::requestData(std::string websiteUrl) {

  if (!validateUrl(websiteUrl) || websiteUrl.size() == 0) {
    throw CustomException((char *)("not a valid url"));
  }

  CURL *curl{};
  CURLcode res{};

  std::string s;

  curl = curl_easy_init();

  if (curl) {

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackToWrite);
    curl_easy_setopt(curl, CURLOPT_URL, websiteUrl.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, answer.c_str());

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      throw CustomException((char *)(curl_easy_strerror(res)));
    }
  }

  curl_easy_cleanup(curl);
  response = new std::string{answer};

  return response;
}

Downloader::~Downloader() {
  std::cout << "deleted downloader\n";

  delete response;
}

Downloader::Downloader() { response = nullptr; };
