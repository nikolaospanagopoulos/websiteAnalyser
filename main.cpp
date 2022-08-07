#include "CustomException.h"
#include "Downloader.h"
#include "HtmlParser.h"
#include <iostream>
int main() {

  try {
    std::string *websiteHtmlContent{};

    Downloader downloader{};

    websiteHtmlContent = downloader.requestData(
        "https://www.bbc.com/news/world-middle-east-62445951");

    HtmlParser parser{};
    parser.removeSpaces(*websiteHtmlContent);

    parser.removeBeyondBodyContent(*websiteHtmlContent);

    parser.removeScriptTags(*websiteHtmlContent);
    parser.removeHtmlTags(*websiteHtmlContent);
    parser.removeSpecialChars(websiteHtmlContent);
    std::cout << "size: " << *websiteHtmlContent << std::endl;

    delete websiteHtmlContent;
    websiteHtmlContent = nullptr;

    std::cout << "nikos";
  } catch (CustomException &err) {
    std::cerr << err.what() << std::endl;
  }

  return 0;
}
