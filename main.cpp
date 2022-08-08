#include "App.hpp"
#include "CustomException.hpp"
#include "Downloader.hpp"
#include "HtmlParser.hpp"
#include <iostream>
int main() {

  try {

    std::string *websiteHtmlContent = nullptr;

    Downloader downloader{};

    std::cout << "choose a website:\n";
    std::string website{};

    std::cin >> website;

    websiteHtmlContent = downloader.requestData(website);
    HtmlParser parser{};
    parser.removeSpaces(*websiteHtmlContent);

    parser.removeBeyondBodyContent(*websiteHtmlContent);

    parser.removeScriptTags(*websiteHtmlContent);
    parser.removeHtmlTags(*websiteHtmlContent);
    parser.removeComments(websiteHtmlContent);
    parser.removeSpecialChars(websiteHtmlContent);

    parser.seperateWordsOnCapital(&websiteHtmlContent);

    delete websiteHtmlContent;
  } catch (CustomException &err) {
    std::cerr << err.what() << std::endl;
  }

  return 0;
}
