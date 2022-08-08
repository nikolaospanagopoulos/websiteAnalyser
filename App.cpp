#include "App.hpp"
#include "Downloader.hpp"
#include "HtmlParser.hpp"

std::string *App::prepareHtmlData() {

  std::string *websiteHtmlContent = nullptr;
  Downloader downloader{};

  websiteHtmlContent = downloader.requestData(website);
  HtmlParser parser{};
  parser.removeSpaces(*websiteHtmlContent);

  parser.removeBeyondBodyContent(*websiteHtmlContent);

  parser.removeScriptTags(*websiteHtmlContent);
  parser.removeHtmlTags(*websiteHtmlContent);
  parser.removeComments(websiteHtmlContent);
  parser.removeSpecialChars(websiteHtmlContent);

  parser.seperateWordsOnCapital(&websiteHtmlContent);
  return websiteHtmlContent;
}
