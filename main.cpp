#include "CustomException.hpp"
#include "Downloader.hpp"
#include "HtmlParser.hpp"
#include "helpers.h"
#include <iostream>
int main() {

  try {

    Downloader downloader{};

    std::string website = getUrlFromUser();

    std::string *websiteHtmlContent = downloader.requestData(website);
    HtmlParser parser{};

    parser.prepareDataForVector(websiteHtmlContent);


    std::vector<std::string> *wordVector =
        parser.fillVector(websiteHtmlContent);

    for (auto &val : *wordVector) {
      std::cout << val << '\n';
    }

  } catch (CustomException &err) {
    std::cerr << err.what() << std::endl;
  }

  return 0;
}
