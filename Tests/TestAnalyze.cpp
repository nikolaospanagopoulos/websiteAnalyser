
#include "../CustomException.hpp"
#include "../Database.hpp"
#include "../Downloader.hpp"
#include "../HtmlParser.hpp"
#include "../JsonDownloader.hpp"
#include "../JsonResponse.h"
#include "../analyzeWebsite.h"
#include "../crow/middlewares/cors.h"
#include "../helpers.h"
#include <crow.h>
#include <iostream>
#include <mariadb/conncpp/Exception.hpp>
int main() {

  try {
    Database *db = new Database{};
    try {
      Downloader *downloader = new Downloader{};
      HtmlParser *parser = new HtmlParser{};
      JsonDownloader *wordsDownloader = new JsonDownloader{};
      json response = analyzeWebsite(
          downloader, parser, wordsDownloader,
          (std::string)(
              "https://www.theguardian.com/world/live/2022/aug/23/"
              "russia-ukraine-war-nuclear-sabre-rattling-must-stop-un-chief-"
              "warns-us-predicts-moscow-to-step-up-strikes-live"),
          db);
    } catch (CustomException &e) {
      json errorMessage = createErrorResponse(e.what());
    } catch (const std::exception &ex) {
      json errorMessage = createErrorResponse(ex.what());
    }
    delete db;

  } catch (CustomException &err) {
    std::cerr << err.what() << std::endl;
  } catch (sql::SQLException &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
