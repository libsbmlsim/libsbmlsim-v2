#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_ASTNODEWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_ASTNODEWRAPPER_H_

#include <sbml/SBMLTypes.h>

LIBSBML_CPP_NAMESPACE_USE

class ASTNodeWrapper {
 public:
  explicit ASTNodeWrapper(const ASTNode *node);
  ASTNodeWrapper(const ASTNodeWrapper &node);
  ~ASTNodeWrapper();
 private:
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_ASTNODEWRAPPER_H_ */
