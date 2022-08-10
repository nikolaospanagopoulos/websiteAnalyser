#include "HtmlParser.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <regex>
#include <sstream>

bool specialChars(char &x) { return !std::isalpha(x) && x != ' '; }

bool (*speacialCharsFunc)(char &) = specialChars;

void HtmlParser::removeSpecialChars(std::string *html) {

  auto beginToRemove =
      std::remove_if(html->begin(), html->end(), speacialCharsFunc);

  html->erase(beginToRemove, html->end());
}

void HtmlParser::removeSpaces(std::string *html) {
  std::string::iterator newEnd =
      std::unique(html->begin(), html->end(), [](char &x, char &y) {
        return (std::isspace(x) && std::isspace(y));
      });
  if (newEnd != html->end()) {
    html->erase(newEnd, html->end());
  }
}

void HtmlParser::removeBeyondBodyContent(std::string *html) {

  size_t firstBodyTag = html->find("<body");

  html->erase(0, firstBodyTag);
  long long int lastBodyTag = html->find_last_of("</body");
  html->erase(lastBodyTag);
}

void HtmlParser::removeHtmlTags(std::string *html) {

  std::regex tags("<[^>]*>");

  *html = std::regex_replace(*html, tags, "");
}

void HtmlParser::seperateWordsOnCapital(std::string **data) {

  std::string *seperatedWords = new std::string{};

  for (size_t i{}; i < (*data)->size(); i++) {
    if (std::isupper((**data)[i]) && i > 0 && (**data)[i - 1] != ' ' &&
        std::islower((**data)[i - 1])) {
      *seperatedWords += " ";
    }
    *seperatedWords += (**data)[i];
  }

  **data = *seperatedWords;
  delete seperatedWords;
}

void HtmlParser::removeScriptTags(std::string *html) {

  std::string::iterator start = html->begin();

  while (start != html->end()) {
    size_t firstScript = html->find("<script");
    size_t lastScript = html->find("</script>");

    if (firstScript != std::string::npos && lastScript != std::string::npos &&
        lastScript > firstScript) {

      std::string scriptString =
          html->substr(firstScript, lastScript - firstScript);
      html->erase(firstScript, scriptString.size());
    }
    start++;
  }
}
void HtmlParser::removeComments(std::string *html)

{

  std::string::iterator startIter = html->begin();

  size_t firstComment = html->find("/*");
  size_t lastComment = html->find("*/");
  while (startIter != html->end()) {

    if (firstComment != std::string::npos && lastComment != std::string::npos &&
        lastComment > firstComment) {
      std::string commentString =
          html->substr(firstComment, lastComment - firstComment);
      html->erase(firstComment, commentString.size());
    }
    if (firstComment != std::string::npos && lastComment == std::string::npos) {

      html->erase(firstComment, std::string::npos);
    }

    startIter++;
  }
}

void HtmlParser::prepareDataForVector(std::string *html) {

  removeSpaces(html);

  removeBeyondBodyContent(html);

  removeScriptTags(html);
  removeHtmlTags(html);
  removeComments(html);
  removeSpecialChars(html);

  seperateWordsOnCapital(&html);
}

HtmlParser::HtmlParser() { words = new std::vector<std::string>{}; }

HtmlParser::~HtmlParser() { delete words; }

std::vector<std::string> *HtmlParser::fillVector(std::string *html) {

  std::stringstream *wordStream = new std::stringstream{*html};

  std::string *temp = new std::string{};

  while (*wordStream >> *temp) {
    if (temp->size() >= 20 || temp->size() < 3) {

      continue;
    }
    words->push_back(*temp);
  }
  delete temp;
  delete wordStream;
  return words;
}
