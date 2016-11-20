#ifndef INCLUDE_SBMLSIM_INTERNAL_UTIL_DEVUTIL_H_
#define INCLUDE_SBMLSIM_INTERNAL_UTIL_DEVUTIL_H_

#include <sbml/SBMLTypes.h>

class DevUtil {
 public:
  static void dumpSBMLSpecInfo(const SBMLDocument *document);
  static void dumpSBMLDocument(const SBMLDocument *document);
  static void dumpASTNode(const ASTNode *node);
 private:
  DevUtil();
  ~DevUtil();
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_UTIL_DEVUTIL_H_ */
