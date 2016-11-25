#include "sbmlsim/internal/util/RuntimeExceptionUtil.h"
#include <stdexcept>
#include <sstream>

void RuntimeExceptionUtil::throwUnknownNodeTypeException(int nodeType) {
  std::stringstream ss;
  ss << "[RuntimeException] Unknown node type: " << nodeType;
  throwRuntimeException(ss.str());
}

void RuntimeExceptionUtil::throwUnknownNodeNameException(const std::string &nodeName) {
  throwRuntimeException("[RuntimeException] Unknown node name: " + nodeName);
}

void RuntimeExceptionUtil::throwInvalidFlowException() {
  throwRuntimeException("[RuntimeException] Invalid flow");
}

void RuntimeExceptionUtil::throwArithmeticException() {
  throwRuntimeException("[RuntimeException] Arithmetic exception");
}

void RuntimeExceptionUtil::throwRuntimeException(const std::string &message) {
  throw std::runtime_error(message);
}
