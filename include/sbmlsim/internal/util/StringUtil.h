#ifndef INCLUDE_SBMLSIM_INTERNAL_UTIL_STRINGUTIL_H_
#define INCLUDE_SBMLSIM_INTERNAL_UTIL_STRINGUTIL_H_

#include <string>
#include <vector>

class StringUtil {
 public:
  std::vector<std::string> split(const std::string &s, const char delim);
  std::string trim(const std::string &s);
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_UTIL_STRINGUTIL_H_ */
