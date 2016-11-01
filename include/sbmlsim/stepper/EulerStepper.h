#ifndef INCLUDE_SBMLSIM_STEPPER_EULERSTEPPER_H_
#define INCLUDE_SBMLSIM_STEPPER_EULERSTEPPER_H_

#include "sbmlsim/stepper/IStepper.h"

class EulerStepper : IStepper {
 public:
  EulerStepper();
  virtual ~EulerStepper();
  virtual void do_step(const SpeciesState &x, SpeciesState &dx,
      double t, double dt,
      const Reaction *reaction, const ParameterState &parameterState);
};

#endif /* INCLUDE_SBMLSIM_STEPPER_EULERSTEPPER_H_ */
