#pragma once
#include "json.h"

using json = nlohmann::json;

json createJsonRespone(const std::vector<std::string *> *responsesVec);
json createErrorResponse(std::string errorMessage);
