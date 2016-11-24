#include "sbmlsim/internal/util/ASTNodeUtil.h"

ASTNode *ASTNodeUtil::rewriteFunctionDefinition(const ASTNode *node,
                                                const ListOfFunctionDefinitions *functionDefinitions) {
  ASTNode *ret;

  if (node->getType() == AST_FUNCTION) {
    // replace arguments of functionDefinition
    auto name = node->getName();
    for (auto i = 0; i < functionDefinitions->size(); i++) {
      auto fd = functionDefinitions->get(i);
      if (name == fd->getName()) {
        ret = fd->getBody()->deepCopy();
        for (auto j = 0; j < fd->getNumArguments(); j++) {
          auto argument = fd->getArgument(j);
          ret->replaceArgument(argument->getName(), node->getChild(j));
        }
        break;
      }
    }
  } else {
    ret = node->deepCopy();
  }

  // rewrite children's function node recursively
  for (auto i = 0; i < ret->getNumChildren(); i++) {
    auto newChild = rewriteFunctionDefinition(ret->getChild(i), functionDefinitions);
    ret->replaceChild(i, newChild, true);
  }

  return ret;
}
