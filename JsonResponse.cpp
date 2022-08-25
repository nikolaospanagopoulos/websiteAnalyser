#include "JsonResponse.h"
#include <iostream>
json createJsonRespone(const std::vector<std::string *> *responsesVec,
                       std::map<std::string, double> *percentageMap) {

  json j;

  j["status"] = "ok";
  // key `rooms` and create the json array from the vector:
  if (responsesVec->empty()) {
    return j["response"] = "no data available";
  }
  std::vector<std::string> *forJson = new std::vector<std::string>{};
  for (auto ptr : *responsesVec) {
    forJson->push_back(*ptr);
  }
  j["response"] = *forJson;

  j["percentages"] = *percentageMap;
  std::cout << j.dump() << '\n';
  delete forJson;
  return j;
}

json createErrorResponse(std::string errorMessage) {

  json errorJson{};

  errorJson["error"] = errorMessage;

  return errorJson;
}
