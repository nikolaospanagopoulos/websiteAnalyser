#include "HtmlParser.h"
#include <algorithm>
#include <cstddef>
#include <regex>

bool specialChars(char &x) { return !std::isalpha(x) && x != ' '; }

bool (*speacialCharsFunc)(char &) = specialChars;

void HtmlParser::removeSpecialChars(std::string *html) {

  auto beginToRemove =
      std::remove_if(html->begin(), html->end(), speacialCharsFunc);

  html->erase(beginToRemove, html->end());
}

void HtmlParser::removeSpaces(std::string &html) {
  std::string::iterator newEnd =
      std::unique(html.begin(), html.end(), [](char &x, char &y) {
        return (std::isspace(x) && std::isspace(y));
      });
  if (newEnd != html.end()) {
    html.erase(newEnd, html.end());
  }
}

void HtmlParser::removeBeyondBodyContent(std::string &html) {

  size_t firstBodyTag = html.find("<body");

  std::cout << html.size() << std::endl;
  html.erase(0, firstBodyTag);
  std::cout << html.size() << std::endl;
  long long int lastBodyTag = html.find_last_of("</body");
  html.erase(lastBodyTag);
  std::cout << html.size() << std::endl;
}

void HtmlParser::removeHtmlTags(std::string &html) {

  std::regex tags("<[^>]*>");

  html = std::regex_replace(html, tags, "");
}

void HtmlParser::removeScriptTags(std::string &html) {

  std::string::iterator start = html.begin();

  while (start != html.end()) {
    size_t firstScript = html.find("<script");
    size_t lastScript = html.find("</script>");

    if (firstScript != std::string::npos && lastScript != std::string::npos &&
        lastScript > firstScript) {

      std::string scriptString =
          html.substr(firstScript, lastScript - firstScript);
      html.erase(firstScript, scriptString.size());
    }
    start++;
  }
}
