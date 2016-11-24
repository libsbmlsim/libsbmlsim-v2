#ifndef INCLUDE_SBMLSIM_INTERNAL_OBSERVER_STDOUTCSVOBSERVER_H_
#define INCLUDE_SBMLSIM_INTERNAL_OBSERVER_STDOUTCSVOBSERVER_H_

#include <vector>
#include "sbmlsim/internal/system/SBMLSystem.h"
#include "sbmlsim/internal/observer/ObserveTarget.h"

class StdoutCsvObserver {
 public:
  explicit StdoutCsvObserver(const std::vector<ObserveTarget> &targets);
  StdoutCsvObserver(const StdoutCsvObserver &observer);
  ~StdoutCsvObserver();
  void operator()(const SBMLSystem::state &x, double t);
  void outputState(const SBMLSystem::state &x, double t);
  void outputHeader();
 private:
  std::vector<ObserveTarget> targets;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_OBSERVER_STDOUTCSVOBSERVER_H_ */
