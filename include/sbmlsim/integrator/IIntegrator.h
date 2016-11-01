#ifndef INCLUDE_SBMLSIM_INTEGRATOR_IINTEGRATOR_H_
#define INCLUDE_SBMLSIM_INTEGRATOR_IINTEGRATOR_H_

#include "sbmlsim/solver/ISolver.h"
#include "sbmlsim/system/ISystem.h"
#include "sbmlsim/observer/IObserver.h"

class IIntegrator {
 public:
  virtual IIntegrator();
  virtual ~IIntegrator();
  virtual void operator()(const ISolver& solver,
                          const ISystem &system,
                          const IObserver &observer,
                          double startTime,
                          double endTime,
                          double dt);
};

#endif /* INCLUDE_SBMLSIM_INTEGRATOR_IINTEGRATOR_H_ */
