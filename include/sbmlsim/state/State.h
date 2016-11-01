#ifndef INCLUDE_SBMLSIM_STATE_STATE_H_
#define INCLUDE_SBMLSIM_STATE_STATE_H_

#include "sbmlsim/state/SpeciesState.h"
#include "sbmlsim/state/ParameterState.h"

class State {
 public:
  State(const SpeciesState &speciesState, const ParameterState &parameterState);
  virtual ~State();
 private:
  SpeciesState speciesState;
  ParameterState parameterState;
};

#endif /* INCLUDE_SBMLSIM_STATE_STATE_H_ */
