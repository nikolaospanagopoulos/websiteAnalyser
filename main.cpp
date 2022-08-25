#include "CustomException.hpp"
#include "Database.hpp"
#include "Downloader.hpp"
#include "HtmlParser.hpp"
#include "JsonDownloader.hpp"
#include "JsonResponse.h"
#include "analyzeWebsite.h"
#include "crow/middlewares/cors.h"
#include "helpers.h"
#include <crow.h>
#include <iostream>
#include <mariadb/conncpp/Exception.hpp>
int main() {

  try {
    Database *db = new Database{};
    crow::App<crow::CORSHandler> app;
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global()
        .headers("Access-Control-Allow-Headers", "*")
        .methods("POST"_method, "GET"_method)
        .prefix("/cors")
        .prefix("/nocors")
        .ignore();

    CROW_ROUTE(app, "/analyze")
        .methods("POST"_method)([db](const crow::request &req) {
          try {
            Downloader *downloader = new Downloader{};
            HtmlParser *parser = new HtmlParser{};
            JsonDownloader *wordsDownloader = new JsonDownloader{};
            auto x = crow::json::load(req.body);
            if (!x) {
              return crow::response(400);
            }
            json response = analyzeWebsite(downloader, parser, wordsDownloader,
                                           (std::string)x["website"], db);
            return crow::response(response.dump());
          } catch (CustomException &e) {
            json errorMessage = createErrorResponse(e.what());
            return crow::response(400, errorMessage.dump());
          } catch (const std::exception &ex) {
            json errorMessage = createErrorResponse(ex.what());
            return crow::response(400, errorMessage.dump());
          }
        });

    CROW_ROUTE(app, "/addwords")
        .methods("POST"_method)([db](const crow::request &req) {
          try {
            auto x = crow::json::load(req.body);
            if (!x["category"]) {
              return crow::response(400);
            }
            std::string category = (std::string)x["category"];
            std::string theme = (std::string)x["theme"];
            json responseJson = db->addWords(category, theme);
            return crow::response(responseJson.dump());
          } catch (CustomException &e) {
            json errorMessage = createErrorResponse(e.what());
            return crow::response(400, errorMessage.dump());
          } catch (const std::exception &ex) {
            json errorMessage = createErrorResponse(ex.what());
            return crow::response(400, errorMessage.dump());
          }
        });

    app.port(5000).multithreaded().run();
    delete db;
    // switch (choice) {

    // case '3':
    // db->showTables();
    // break;
    // case '4':
    //  db->showWordsByCategory();
    // break;
    // case '5':
    // db->deleteCategory();
    // break;
    //}
    //} while (choice != 'x' && choice != 'X');

  } catch (CustomException &err) {
    std::cerr << err.what() << std::endl;
  } catch (sql::SQLException &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
