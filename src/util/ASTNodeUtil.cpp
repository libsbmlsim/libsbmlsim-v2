#include "sbmlsim/internal/util/ASTNodeUtil.h"

#define DELETE_REPLACED_NODE true

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
    ret->replaceChild(i, newChild, DELETE_REPLACED_NODE);
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
    ret->replaceChild(i, newChild, DELETE_REPLACED_NODE);
  }

  return ret;
}

ASTNode *ASTNodeUtil::reduceToBinary(const ASTNode *node) {
  auto type = node->getType();
  auto numChildren = node->getNumChildren();

  // keep piecewise node
  if (type == AST_FUNCTION_PIECEWISE) {
    ASTNode *ret = node->deepCopy();
    for (auto i = 0; i < numChildren; i++) {
      auto newChild = reduceToBinary(ret->getChild(i));
      ret->replaceChild(i, newChild, DELETE_REPLACED_NODE);
    }
    return ret;
  }

  // binary, unary, variable or constant
  if (numChildren <= 2) {
    return node->deepCopy();
  }

  //
  //    F             F
  //  / | \          / \
  // x  y  z  ==>   F   z
  //               / \
  //              x   y
  //

  auto rightNode = reduceToBinary(node->getRightChild());

  auto leftNode = node->deepCopy();
  auto rightOfLeftNode = leftNode->getRightChild();
  leftNode->removeChild(leftNode->getNumChildren() - 1);
  delete rightOfLeftNode;

  auto *ret = node->deepCopy();
  while (ret->getNumChildren() > 0) {
    auto index = ret->getNumChildren() - 1;
    auto child = ret->getChild(index);
    ret->removeChild(index);
    delete child;
  }
  ret->addChild(leftNode);
  ret->addChild(rightNode);

  return ret;
}
