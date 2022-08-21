#pragma once
#include "Database.hpp"
#include "Downloader.hpp"
#include "HtmlParser.hpp"
#include "JsonDownloader.hpp"
#include <string>
#include <vector>

json analyzeWebsite(Downloader *downloader, HtmlParser *parser,
                    JsonDownloader *wordsDownloader, const std::string &website,
                    Database *db);
