#pragma once
#include <algorithm>
#include <iostream>
class HtmlParser {

public:
  void removeSpaces(std::string &html);
  void removeBeyondBodyContent(std::string &html);
  void seperateWordsOnCapital(std::string **data);
  void removeHtmlTags(std::string &html);
  void removeScriptTags(std::string &html);
  void removeSpecialChars(std::string *html);
};
