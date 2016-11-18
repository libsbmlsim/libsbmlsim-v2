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

  // boundary condition
  auto &specieses = model.getSpecieses();
  for (auto i = 0; i < specieses.size(); i++) {
    if (specieses[i].hasBoundaryCondition()) {
      dxdt[i] = 0.0;
    }
  }
}

double SBMLSystem::evaluateASTNode(const ASTNode *node, int reactionIndex, const state& x) {
  double left, right;

  if (node->getLeftChild() != NULL) {
    left = evaluateASTNode(node->getLeftChild(), reactionIndex, x);
  }
  if (node->getRightChild() != NULL) {
    right = evaluateASTNode(node->getRightChild(), reactionIndex, x);
  }

  ASTNodeType_t type = node->getType();
  switch (type) {
    case AST_NAME: {
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
      break;
    }
    case AST_PLUS:
      return left + right;
    case AST_MINUS:
      return left - right;
    case AST_TIMES:
      return left * right;
    case AST_DIVIDE:
      return left / right;
    case AST_REAL:
      return node->getReal();
    case AST_POWER:
    case AST_FUNCTION_POWER:
      return std::pow(left, right);
    case AST_INTEGER:
      return node->getInteger();
    default:
      std::cout << "type = " << type << std::endl;
      break;
  }
  return 0;
}

int SBMLSystem::getIndexForSpecies(const std::string &speciesId) {
  auto specieses = model.getSpecieses();
  for (auto i = 0; i < specieses.size(); i++) {
    if (speciesId == specieses[i].getId()) {
      return i;
    }
  }
}
