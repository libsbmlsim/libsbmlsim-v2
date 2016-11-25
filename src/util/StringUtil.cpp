#include "sbmlsim/internal/util/StringUtil.h"
#include <sstream>

std::vector<std::string> StringUtil::split(const std::string &s, const char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    if (!item.empty()) {
      elems.push_back(item);
    }
  }
  return elems;
}

std::string StringUtil::trim(const std::string &s) {
  auto first = s.find_first_not_of(' ');
  auto last = s.find_last_not_of(' ');
  return s.substr(first, (last - first + 1));
}
