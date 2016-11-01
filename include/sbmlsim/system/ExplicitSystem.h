#ifndef INCLUDE_SBMLSIM_SYSTEM_EXPLICITSYSTEM_H_
#define INCLUDE_SBMLSIM_SYSTEM_EXPLICITSYSTEM_H_

#include "sbmlsim/system/ISystem.h"
#include "sbmlsim/state/SpeciesState.h"
#include "sbmlsim/state/ParameterState.h"

class ExplicitSystem : ISystem {
 public:
  ExplicitSystem() {}
  virtual ~ExplicitSystem() {}
  virtual void operator()(const SpeciesState &x, SpeciesState &dxdt,
      const Reaction *reaction, const ParameterState &parameterState);

 private:
  double evaluateASTNode(const ASTNode *node, const ReactionId &reactionId,
      const SpeciesState &speciesState, const ParameterState &parameterState) const;
};

#endif /* INCLUDE_SBMLSIM_SYSTEM_EXPLICITSYSTEM_H_ */
