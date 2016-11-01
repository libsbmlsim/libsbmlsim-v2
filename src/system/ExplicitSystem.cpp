#include <cassert>
#include "sbmlsim/system/ExplicitSystem.h"
#include "sbmlsim/def.h"

void ExplicitSystem::operator()(
    const SpeciesState &x, SpeciesState &dxdt,
    const Reaction *reaction, const ParameterState &parameterState) {
  const ReactionId reactionId = reaction->getId();

  const KineticLaw *kineticLaw = reaction->getKineticLaw();
  const ASTNode *astNode = kineticLaw->getMath();

  double value = this->evaluateASTNode(astNode, reactionId, x, parameterState);

  unsigned int numReactants = reaction->getNumReactants();
  for (unsigned int i = 0; i < numReactants; i++) {
    const SpeciesReference *ref = reaction->getReactant(i);
    const SpeciesId speciesId = ref->getSpecies();
    dxdt.setSpeciesValue(speciesId, dxdt.getSpeciesValue(speciesId) - value);
  }

  unsigned int numProducts = reaction->getNumProducts();
  for (unsigned int i = 0; i < numProducts; i++) {
    const SpeciesReference *ref = reaction->getProduct(i);
    const SpeciesId speciesId = ref->getSpecies();
    dxdt.setSpeciesValue(speciesId, dxdt.getSpeciesValue(speciesId) + value);
  }
}

double ExplicitSystem::evaluateASTNode(const ASTNode *node, const ReactionId &reactionId,
    const SpeciesState &speciesState, const ParameterState &parameterState) const {
  double left, right;

  if (node->getLeftChild() != NULL) {
    left = evaluateASTNode(node->getLeftChild(), reactionId, speciesState, parameterState);
  }
  if (node->getRightChild() != NULL) {
    right = evaluateASTNode(node->getRightChild(), reactionId, speciesState, parameterState);
  }

  std::string name;
  switch (node->getType()) {
    case AST_NAME:
      name = node->getName();
      if (speciesState.hasSpeciesValue(name)) {
        return speciesState.getSpeciesValue(name);
      }
      if (parameterState.hasParameter(name, reactionId)) {
        return parameterState.getParameterValue(name, reactionId);
      }
      break;
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
    default:
      break;
  }

  assert(false);
  return 0;
}
