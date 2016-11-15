#include "sbmlsim/system/SBMLSystem.h"

SBMLSystem::SBMLSystem(const Model *model)
    : model(model) {
  // nothing to do
}

void SBMLSystem::operator()(const state& x, state& dxdt, double t) {
  // initialize
  for (int i = 0; i < dxdt.size(); i++) {
    dxdt[i] = 0.0;
  }

  for (int i = 0; i < model->getNumReactions(); i++) {
    const Reaction *reaction = model->getReaction(i);
    const KineticLaw *kineticLaw = reaction->getKineticLaw();
    const ASTNode *node = kineticLaw->getMath();
    ASTNode *clonedNode = node->deepCopy();
    clonedNode->reduceToBinary();

    double value = evaluateASTNode(clonedNode, i, x);

    // reactants
    for (int j = 0; j < reaction->getNumReactants(); j++) {
      const SpeciesReference *ref = reaction->getReactant(j);
      const std::string speciesId = ref->getSpecies();
      int index = getIndexForSpecies(speciesId);
      dxdt[index] -= value;
    }

    // products
    for (int j = 0; j < reaction->getNumProducts(); j++) {
      const SpeciesReference *ref = reaction->getProduct(j);
      const std::string speciesId = ref->getSpecies();
      int index = getIndexForSpecies(speciesId);
      dxdt[index] += value;
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
      std::string name = node->getName();
      // species
      for (int i = 0; i < model->getNumSpecies(); i++) {
        const Species *species = model->getSpecies(i);
        if (name == species->getId()) {
          return x[i];
        }
      }
      // compartment
      for (int i = 0; i < model->getNumCompartments(); i++) {
        const Compartment *compartment = model->getCompartment(i);
        if (name == compartment->getId()) {
          return compartment->getSize();
        }
      }
      // local parameter
      const Reaction *reaction = model->getReaction(reactionIndex);
      const KineticLaw *kineticLaw = reaction->getKineticLaw();
      for (int i = 0; i < kineticLaw->getNumLocalParameters(); i++) {
        const Parameter *parameter = kineticLaw->getLocalParameter(i);
        if (name == parameter->getId()) {
          return parameter->getValue();
        }
      }
      // global parameter
      for (int i = 0; i < model->getNumParameters(); i++) {
        const Parameter *parameter = model->getParameter(i);
        if (name == parameter->getId()) {
          return parameter->getValue();
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
    case AST_INTEGER:
      return node->getInteger();
    default:
      assert(false);
      break;
  }
  return 0;
}

int SBMLSystem::getIndexForSpecies(std::string speciesId) {
  for (int i = 0; i < model->getNumSpecies(); i++) {
    const Species *species = model->getSpecies(i);
    if (species->getId() == speciesId) {
      return i;
    }
  }
}
