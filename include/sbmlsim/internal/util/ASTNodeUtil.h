#ifndef INCLUDE_SBMLSIM_INTERNAL_UTIL_ASTNODEUTIL_H_
#define INCLUDE_SBMLSIM_INTERNAL_UTIL_ASTNODEUTIL_H_

#include <sbml/SBMLTypes.h>

class ASTNodeUtil {
 public:
  static ASTNode *rewriteFunctionDefinition(const ASTNode *node, const ListOfFunctionDefinitions *functionDefinitions);
  static ASTNode *rewriteLocalParameters(const ASTNode *node, const ListOfParameters *localParameters);
 private:
  ASTNodeUtil() {}
  ~ASTNodeUtil() {}
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_UTIL_ASTNODEUTIL_H_ */
