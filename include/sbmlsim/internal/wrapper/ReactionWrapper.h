#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_REACTIONWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_REACTIONWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>
#include <vector>
#include "sbmlsim/internal/wrapper/SpeciesReferenceWrapper.h"

class ReactionWrapper {
 public:
  explicit ReactionWrapper(const Reaction *reaction);
  ReactionWrapper(const ReactionWrapper &reaction);
  ~ReactionWrapper();
  const std::string &getId() const;
  const std::vector<SpeciesReferenceWrapper> &getReactants() const;
  const std::vector<SpeciesReferenceWrapper> &getProducts() const;
  const ASTNode *getMath();
 private:
  std::string id;
  std::vector<SpeciesReferenceWrapper> reactants;
  std::vector<SpeciesReferenceWrapper> products;
  ASTNode *math;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_REACTIONWRAPPER_H_ */
