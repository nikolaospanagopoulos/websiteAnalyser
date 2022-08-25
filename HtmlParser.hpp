#pragma once
#include <algorithm>
#include <iostream>
#include <set>
class HtmlParser {

private:
  std::set<std::string> *words{};
  bool checkIfHasCapitalsInside(const std::string *word) const;

  void removeSpaces(std::string *html);
  void removeBeyondBodyContent(std::string *html);
  void seperateWordsOnCapital(std::string **data);
  void removeHtmlTags(std::string *html);
  void removeScriptTags(std::string *html);
  void removeSpecialChars(std::string *html);
  void removeComments(std::string *html);

public:
  void prepareDataForVector(std::string *html);

  std::set<std::string> *fillSet(std::string *html);

  HtmlParser();
  ~HtmlParser();
};
