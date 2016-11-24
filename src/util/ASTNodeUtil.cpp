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

ASTNode *ASTNodeUtil::rewriteLocalParameters(const ASTNode *node, const ListOfParameters *localParameters) {
  ASTNode *ret;

  if (node->getType() == AST_NAME) {
    auto name = node->getName();
    for (auto i = 0; i < localParameters->size(); i++) {
      auto param = localParameters->get(i);
      if (name == param->getId()) {
        ret = new ASTNode(AST_REAL);
        ret->setValue(param->getValue());
        return ret;
      }
    }
  }

  // if the node doesn't represent local parameter

  ret = node->deepCopy();

  // replace children's local parameter node recursively
  for (auto i = 0; i < ret->getNumChildren(); i++) {
    auto newChild = rewriteLocalParameters(ret->getChild(i), localParameters);
    ret->replaceChild(i, newChild, true);
  }

  return ret;
}
