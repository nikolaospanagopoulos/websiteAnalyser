#include "helpers.h"
#include "CustomException.hpp"
#include "Downloader.hpp"
#include <algorithm>
#include <cstddef>
#include <random>
#include <vector>

std::string getUrlFromUser() {

  std::cout << "choose a website:\n";
  std::string website{};

  std::cin >> website;
  return website;
}

std::vector<std::string> *
randomWords(const std::set<std::string> *const allWords) {

  size_t randomWordsSize{10};

  if (allWords->size() < randomWordsSize && allWords->size() > 0) {
    randomWordsSize = allWords->size();

  } else if (allWords->size() == 0) {
    throw CustomException((char *)("there are no words"));
  }

  std::vector<std::string> *randomWordsVector = new std::vector<std::string>{};

  std::sample(allWords->begin(), allWords->end(),
              std::back_inserter(*randomWordsVector), randomWordsSize,
              std::mt19937{std::random_device{}()});

  return randomWordsVector;
}

size_t writeResponseToString(char *contents, size_t size, size_t nmemb,
                             void *userData) {
  size_t newLength = size * nmemb;
  try {

    ((std::string *)userData)->append((char *)contents, newLength);

  } catch (std::bad_alloc &e) {
    std::cerr << e.what() << std::endl;
  }

  return newLength;
}
size_t (*callBackToWrite)(char *, size_t, size_t,
                          void *) = writeResponseToString;
std::vector<std::string *> *analyzeWebsite(Downloader *downloader,
                                           HtmlParser *parser,
                                           JsonDownloader *wordsDownloader) {

  std::string website = getUrlFromUser();

  std::string *websiteHtmlContent = downloader->requestData(website);

  parser->prepareDataForVector(websiteHtmlContent);

  std::set<std::string> *wordSet = parser->fillSet(websiteHtmlContent);

  std::vector<std::string> *randomWordsVector = randomWords(wordSet);

  for (const auto &el : *randomWordsVector) {
    wordsDownloader->fillJsonVector(el);
  }

  std::vector<std::string *> *resultsWords =
      wordsDownloader->getWordsFromJson();

  delete randomWordsVector;
  return resultsWords;
}

void freeMemory(std::vector<std::string *> *ids, HtmlParser *parser,
                JsonDownloader *wordsDownloader,
                std::vector<std::string *> *resultsWords,
                Downloader *downloader,
                std::vector<std::string *> *finalResults) {

  delete wordsDownloader;
  delete downloader;
  delete parser;
  for (auto ptr : *finalResults) {
    delete ptr;
  }
  delete finalResults;
  for (auto ptr : *ids) {
    delete ptr;
  }
  delete ids;
  for (auto val : *resultsWords) {

    delete val;
  }
  delete resultsWords;
}

void printVector(const std::vector<std::string *> *results) {
  for (auto res : *results) {
    std::cout << "| " << *res << " |" << std::endl;
  }
}
