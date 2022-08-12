#pragma once
#include "Downloader.hpp"
#include "HtmlParser.hpp"
#include "JsonDownloader.hpp"
#include <iostream>
#include <set>
#include <vector>
std::string getUrlFromUser();

std::vector<std::string> *
randomWords(const std::set<std::string> *const allWords);
size_t writeResponseToString(char *contents, size_t size, size_t nmemb,
                             void *userData);
extern size_t (*callBackToWrite)(char *, size_t, size_t, void *);
std::vector<std::string *> *analyzeWebsite(Downloader *downloader,
                                           HtmlParser *parser,
                                           JsonDownloader *wordsDownloader);
