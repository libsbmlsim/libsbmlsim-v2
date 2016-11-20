#ifndef INCLUDE_SBMLSIM_INTERNAL_OBSERVER_STDOUTCSVOBSERVER_H_
#define INCLUDE_SBMLSIM_INTERNAL_OBSERVER_STDOUTCSVOBSERVER_H_

#include "sbmlsim/internal/system/SBMLSystem.h"

class StdoutCsvObserver {
 public:
  void operator()(const SBMLSystem::state &x, double t);
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_OBSERVER_STDOUTCSVOBSERVER_H_ */
