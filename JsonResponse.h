#pragma once
#include "json.h"

using json = nlohmann::json;

json createJsonRespone(const std::vector<std::string *> *responsesVec,
                       std::map<std::string, double> *percentageMap);
json createErrorResponse(std::string errorMessage);
