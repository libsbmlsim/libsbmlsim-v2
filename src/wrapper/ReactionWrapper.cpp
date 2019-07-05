#include "sbmlsim/internal/wrapper/ReactionWrapper.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"

ReactionWrapper::ReactionWrapper(const Reaction *reaction) {
  this->id = reaction->getId();
  for (auto i = 0; i < reaction->getNumReactants(); i++) {
    auto reactant = reaction->getReactant(i);
    this->reactants.push_back(SpeciesReferenceWrapper(reactant));
  }
  for (auto i = 0; i < reaction->getNumProducts(); i++) {
    auto product = reaction->getProduct(i);
    this->products.push_back(SpeciesReferenceWrapper(product));
  }

  auto node = reaction->getKineticLaw()->getMath();
  auto model = reaction->getModel();

  // Ici
  auto fdRewritedNode = ASTNodeUtil::rewriteFunctionDefinition(node, model->getListOfFunctionDefinitions());
  this->math = ASTNodeUtil::rewriteLocalParameters(fdRewritedNode, reaction->getKineticLaw()->getListOfParameters());
  this->math = ASTNodeUtil::rewriteFamousConstants(fdRewritedNode);
  delete fdRewritedNode;
}

ReactionWrapper::ReactionWrapper(const ReactionWrapper &reaction) {
  this->id = reaction.id;
  this->reactants = reaction.reactants;
  this->products = reaction.products;
  this->math = reaction.math->deepCopy();
}

ReactionWrapper::~ReactionWrapper() {
  this->reactants.clear();
  this->products.clear();
  delete this->math;
}

const std::string &ReactionWrapper::getId() const {
  return this->id;
}

const std::vector<SpeciesReferenceWrapper> &ReactionWrapper::getReactants() const {
  return this->reactants;
}

const std::vector<SpeciesReferenceWrapper> &ReactionWrapper::getProducts() const {
  return this->products;
}

const ASTNode *ReactionWrapper::getMath() {
  return this->math;
}
