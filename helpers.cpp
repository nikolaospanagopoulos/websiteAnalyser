#include "helpers.h"
#include "CustomException.hpp"
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

  size_t randomWordsSize{5};

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
