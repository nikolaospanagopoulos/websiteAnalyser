#include "helpers.h"

std::string getUrlFromUser() {

  std::cout << "choose a website:\n";
  std::string website{};

  std::cin >> website;
  return website;
}
