#include "analyzeWebsite.h"
#include "JsonResponse.h"
#include "helpers.h"
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

  std::vector<std::string *> *resultsWords =
      wordsDownloader->getWordsFromJson();

  delete randomWordsVector;
  std::vector<std::string *> *ids = db->analyzeResults(resultsWords);
  std::vector<std::string *> *finalResults = db->getResults(ids);

  json response = createJsonRespone(finalResults);
  freeMemory(ids, parser, wordsDownloader, resultsWords, downloader,
             finalResults);

  return response;
}
