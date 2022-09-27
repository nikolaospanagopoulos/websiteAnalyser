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
            auto x = crow::json::load(req.body);
            if (!x) {
              return crow::response(400);
            }

            int websiteID = db->getWebsiteId((std::string)x["website"]);

            if (websiteID != -1) {
              json cachedResponse =
                  db->getDbResponse((std::string)x["website"]);
              return crow::response(cachedResponse.dump());
            }

            Downloader *downloader = new Downloader{};
            HtmlParser *parser = new HtmlParser{};
            JsonDownloader *wordsDownloader = new JsonDownloader{};
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

    CROW_ROUTE(app, "/deletecategory")
        .methods("DELETE"_method)([db](const crow::request &req) {
          try {
            auto x = crow::json::load(req.body);
            if (!x["category"]) {
              return crow::response(400);
            }
            std::string category = (std::string)x["category"];
            db->deleteCategory(category);
            json categoryDeletedResponse{};
            categoryDeletedResponse["status"] = "ok";

            return crow::response(204, categoryDeletedResponse.dump());

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

  } catch (CustomException &err) {
    std::cerr << err.what() << std::endl;
  } catch (sql::SQLException &e) {
    std::cerr << e.what() << std::endl;
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << std::endl;
  }
  return 0;
}
