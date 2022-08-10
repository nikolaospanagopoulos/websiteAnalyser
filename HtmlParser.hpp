#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
class HtmlParser {

private:
  std::vector<std::string> *words{};

public:
  void removeSpaces(std::string *html);
  void removeBeyondBodyContent(std::string *html);
  void seperateWordsOnCapital(std::string **data);
  void removeHtmlTags(std::string *html);
  void removeScriptTags(std::string *html);
  void removeSpecialChars(std::string *html);
  void removeComments(std::string *html);
  void prepareDataForVector(std::string *html);

  std::vector<std::string> *fillVector(std::string *html);

  HtmlParser();
  ~HtmlParser();
};
