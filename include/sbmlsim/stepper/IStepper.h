#ifndef INCLUDE_SBMLSIM_STEPPER_ISTEPPER_H_
#define INCLUDE_SBMLSIM_STEPPER_ISTEPPER_H_

#include <sbml/SBMLTypes.h>
#include "sbmlsim/state/SpeciesState.h"
#include "sbmlsim/state/ParameterState.h"

class IStepper {
 public:
  IStepper() {}
  virtual ~IStepper() {}
  virtual void do_step(const SpeciesState &x, SpeciesState &dx,
      double t, double dt,
      const Reaction *reaction, const ParameterState &parameterState) {}
};

#endif /* INCLUDE_SBMLSIM_STEPPER_ISTEPPER_H_ */
