#include "CustomException.hpp"
#include "Database.hpp"
#include "Downloader.hpp"
#include "HtmlParser.hpp"
#include "JsonDownloader.hpp"
#include "helpers.h"
#include <iostream>
#include <mariadb/conncpp/Exception.hpp>
int main() {

  try {

    Database *db = new Database{};

    char choice{};
    do {

      std::cout << std::endl;
      std::cout << "1: find what the website is about" << std::endl;
      std::cout << "2: add words to semantic Category" << std::endl;
      std::cout << "3: show database" << std::endl;
      std::cout << "4: see words by category " << std::endl;
      std::cout << "5: delete a category and all its words" << std::endl;
      std::cout << "X: exit" << std::endl;

      std::cin >> choice;

      switch (choice) {

      case '1': {
        Downloader *downloader = new Downloader{};
        HtmlParser *parser = new HtmlParser{};
        JsonDownloader *wordsDownloader = new JsonDownloader{};
        std::vector<std::string *> *resultsWords =
            analyzeWebsite(downloader, parser, wordsDownloader);
        std::vector<std::string *> *ids = db->analyzeResults(resultsWords);
        std::vector<std::string *> *finalResults = db->getResults(ids);

        printVector(finalResults);
        freeMemory(ids, parser, wordsDownloader, resultsWords, downloader,
                   finalResults);

      } break;
      case '2':
        db->addWords();
        break;

      case '3':
        db->showTables();
        break;
      case '4':
        db->showWordsByCategory();
        break;
      case '5':
        db->deleteCategory();
        break;
      }

    } while (choice != 'x' && choice != 'X');

    delete db;
  } catch (CustomException &err) {
    std::cerr << err.what() << std::endl;
  } catch (sql::SQLException &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
