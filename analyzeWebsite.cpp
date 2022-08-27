#include "analyzeWebsite.h"
#include "JsonDownloader.hpp"
#include "JsonResponse.h"
#include "helpers.h"
#include <thread>
json analyzeWebsite(Downloader *downloader, HtmlParser *parser,
                    JsonDownloader *wordsDownloader, const std::string &website,
                    Database *db) {

  std::string *websiteHtmlContent = downloader->requestData(website);
  parser->prepareDataForVector(websiteHtmlContent);

  std::set<std::string> *wordSet = parser->fillSet(websiteHtmlContent);

  std::vector<std::string> *randomWordsVector = randomWords(wordSet);

  for (const auto &el : *randomWordsVector) {
    wordsDownloader->fillJsonVector(el);
    std::cout << el << std::endl;
  }

  std::vector<std::string> *resultsWords = wordsDownloader->getWordsFromJson();

  delete randomWordsVector;
  std::map<std::string, size_t> *results = db->analyzeResults(resultsWords);
  std::vector<std::string *> *finalResults = db->getResults(results);

  std::map<std::string, double> *percentageMap =
      db->calculatePercentage(results);

  db->insertWebsiteToDb(website, percentageMap, finalResults);

  json response = createJsonRespone(finalResults, percentageMap);
  freeMemory(results, parser, wordsDownloader, resultsWords, downloader,
             finalResults);
  delete percentageMap;

  return response;
}
