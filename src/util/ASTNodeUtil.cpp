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

  auto leftNode = node->deepCopy();
  auto rightOfLeftNode = leftNode->getRightChild();
  leftNode->removeChild(leftNode->getNumChildren() - 1);
  delete rightOfLeftNode;

  auto ret = node->deepCopy();
  while (ret->getNumChildren() > 0) {
    auto index = ret->getNumChildren() - 1;
    auto child = ret->getChild(index);
    ret->removeChild(index);
    delete child;
  }
  ret->addChild(reduceToBinary(leftNode));
  ret->addChild(reduceToBinary(node->getRightChild()));
  delete leftNode;

  return ret;
}

bool ASTNodeUtil::isEqual(const ASTNode *ast1, const ASTNode *ast2) {
  // accept nullptr for ast1 and ast2.
  if (ast1 == nullptr && ast2 == nullptr) {
    return true;
  } else if (ast1 == nullptr) { // ast2 != nullptr
    return false;
  } else if (ast2 == nullptr) { // ast1 != nullptr
    return false;
  }
  bool equal = true;
  equal &= ast1->getType() == ast2->getType();

  if (ast1->isInteger() && ast2->isInteger()) {
    equal &= ast2->getInteger() == ast1->getInteger();
  }
  if (ast1->isName() && ast2->isName()) {
    equal &= (std::strcmp(ast1->getName(), ast2->getName()) == 0);
  }
  if (ast1->isRational() && ast2->isRational()) {
    equal &= ast2->getNumerator() == ast1->getNumerator()
             && ast2->getDenominator() == ast1->getDenominator();
  }
  if ((ast2->getType() == AST_REAL_E) && (ast1->getType() == AST_REAL_E)) {
    equal &= isnan(ast1->getMantissa()) ? isnan(ast2->getMantissa()) : ast2->getMantissa() == ast1->getMantissa()
                                                                       && ast2->getExponent() == ast1->getExponent();
  } else if (ast1->isReal() && ast2->isReal()) {
    equal &= isnan(ast1->getReal()) ? isnan(ast2->getReal()) : ast2->getReal() == ast1->getReal();
  }

  equal &= ast1->isSetId() == ast2->isSetId();
  if (equal && ast1->isSetId()) {
    equal &= ast1->getId() == ast2->getId();
  }
  equal &= ast1->isSetStyle() == ast2->isSetStyle();
  if (equal && ast1->isSetStyle()) {
    equal &= ast1->getStyle() == ast2->getStyle();
  }
  equal &= ast1->isSetUnits() == ast2->isSetUnits();
  if (equal && ast1->isSetUnits()) {
    equal &= ast1->getUnits() == ast2->getUnits();
  }
  return equal;
}
