#ifndef INCLUDE_SBMLSIM_INTERNAL_UTIL_RUNTIMEEXCEPTIONUTIL_H_
#define INCLUDE_SBMLSIM_INTERNAL_UTIL_RUNTIMEEXCEPTIONUTIL_H_

#include <string>

class RuntimeExceptionUtil {
 public:
  static void throwUnknownNodeTypeException(int nodeType);
  static void throwUnknownNodeNameException(const std::string &nodeName);
  static void throwInvalidFlowException();
  private:
  static void throwRuntimeException(const std::string &message);
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_UTIL_RUNTIMEEXCEPTIONUTIL_H_ */
