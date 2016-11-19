#include "sbmlsim/system/SBMLSystem.h"
#include <algorithm>
#include <cmath>

SBMLSystem::SBMLSystem(const ModelWrapper &model)
    : model(model) {
  // nothing to do
}

void SBMLSystem::operator()(const state& x, state& dxdt, double t) {
  // initialize
  for (auto i = 0; i < dxdt.size(); i++) {
    dxdt[i] = 0.0;
  }

  auto reactions = model.getReactions();
  for (auto i = 0; i < reactions.size(); i++) {
    auto reaction = reactions[i];
    auto node = reaction.getMath();
    auto clonedNode = node->deepCopy();
    clonedNode->reduceToBinary();

    auto value = evaluateASTNode(clonedNode, i, x);

    // reactants
    for (auto reactant : reaction.getReactants()) {
      auto index = getIndexForSpecies(reactant.getSpeciesId());
      dxdt[index] -= value * reactant.getStoichiometry();
    }

    // products
    for (auto product : reaction.getProducts()) {
      auto index = getIndexForSpecies(product.getSpeciesId());
      dxdt[index] += value * product.getStoichiometry();
    }
  }

  // boundaryCondition and constant
  auto &specieses = model.getSpecieses();
  for (auto i = 0; i < specieses.size(); i++) {
    if (specieses[i].hasBoundaryCondition() || specieses[i].isConstant()) {
      dxdt[i] = 0.0;
    }
  }
}

double SBMLSystem::evaluateASTNode(const ASTNode *node, int reactionIndex, const state& x) {
  double left, right;

  ASTNodeType_t type = node->getType();
  switch (type) {
    case AST_NAME:
      return evaluateNameNode(node, reactionIndex, x);
    case AST_PLUS:
      left = evaluateASTNode(node->getLeftChild(), reactionIndex, x);
      right = evaluateASTNode(node->getRightChild(), reactionIndex, x);
      return left + right;
    case AST_MINUS:
      left = evaluateASTNode(node->getLeftChild(), reactionIndex, x);
      right = evaluateASTNode(node->getRightChild(), reactionIndex, x);
      return left - right;
    case AST_TIMES:
      left = evaluateASTNode(node->getLeftChild(), reactionIndex, x);
      right = evaluateASTNode(node->getRightChild(), reactionIndex, x);
      return left * right;
    case AST_DIVIDE:
      left = evaluateASTNode(node->getLeftChild(), reactionIndex, x);
      right = evaluateASTNode(node->getRightChild(), reactionIndex, x);
      return left / right;
    case AST_POWER:
    case AST_FUNCTION_POWER:
      left = evaluateASTNode(node->getLeftChild(), reactionIndex, x);
      right = evaluateASTNode(node->getRightChild(), reactionIndex, x);
      return std::pow(left, right);
    case AST_FUNCTION:
      return evaluateFunctionNode(node, reactionIndex, x);
    case AST_REAL:
      return node->getReal();
    case AST_INTEGER:
      return node->getInteger();
    default:
      std::cout << "type = " << type << std::endl;
      break;
  }
  return 0;
}

double SBMLSystem::evaluateNameNode(const ASTNode *node, int reactionIndex, const state &x) {
  auto name = node->getName();

  // species
  auto specieses = model.getSpecieses();
  for (auto i = 0; i < specieses.size(); i++) {
    if (name == specieses[i].getId()) {
      return x[i];
    }
  }

  // compartment
  auto compartments = model.getCompartments();
  for (auto i = 0; i < compartments.size(); i++) {
    if (name == compartments[i].getId()) {
      return compartments[i].getValue();
    }
  }

  // parameter
  auto reactionId = model.getReactions().at(reactionIndex).getId();
  auto parameters = model.getParameters();
  for (auto i = 0; i < parameters.size(); i++) {
    if (parameters[i].isLocalParameter()
        && name == parameters[i].getId()
        && reactionId == parameters[i].getReactionId()) {
      return parameters[i].getValue();
    }
  }
  for (auto i = 0; i < parameters.size(); i++) {
    if (parameters[i].isGlobalParameter() && name == parameters[i].getId()) {
      return parameters[i].getValue();
    }
  }

  return 0.0;
}

double SBMLSystem::evaluateFunctionNode(const ASTNode *node, int reactionIndex, const state &x) {
  auto name = node->getName();
  for (auto functionDefinition : model.getFunctionDefinitions()) {
    if (name == functionDefinition.getName()) {
      auto body = functionDefinition.getBody()->deepCopy();
      for (auto i = 0; i < body->getNumChildren(); i++) {
        auto bvar = body->getChild(i)->getName();
        body->replaceArgument(bvar, node->getChild(i));
      }
      auto value = evaluateASTNode(body, reactionIndex, x);
      delete body;
      return value;
    }
  }

  // not reachable
  return 0.0;
}

int SBMLSystem::getIndexForSpecies(const std::string &speciesId) {
  auto specieses = model.getSpecieses();
  for (auto i = 0; i < specieses.size(); i++) {
    if (speciesId == specieses[i].getId()) {
      return i;
    }
  }
}
