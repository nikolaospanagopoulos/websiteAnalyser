#include "CustomException.hpp"
#include "Database.hpp"
#include "Downloader.hpp"
#include "HtmlParser.hpp"
#include "JsonDownloader.hpp"
#include "helpers.h"
#include <iostream>
int main() {

  try {

    Downloader *downloader = new Downloader{};

    std::string website = getUrlFromUser();

    std::string *websiteHtmlContent = downloader->requestData(website);
    HtmlParser *parser = new HtmlParser{};

    parser->prepareDataForVector(websiteHtmlContent);

    std::set<std::string> *wordSet = parser->fillSet(websiteHtmlContent);

    std::vector<std::string> *randomWordsVector = randomWords(wordSet);

    JsonDownloader *wordsDownloader = new JsonDownloader{};

    for (const auto &el : *randomWordsVector) {
      wordsDownloader->fillJsonVector(el);
    }

    std::vector<json *> *responses = wordsDownloader->getJsonResponses();

    for (auto *jsonRes : *responses) {
      std::cout << jsonRes->dump(4) << std::endl;
    }

    Database *db = new Database{};

    delete db;
    delete randomWordsVector;
    delete wordsDownloader;
    delete downloader;
    delete parser;
  } catch (CustomException &err) {
    std::cerr << err.what() << std::endl;
  }

  return 0;
}
