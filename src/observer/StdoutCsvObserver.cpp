#include "sbmlsim/internal/observer/StdoutCsvObserver.h"
#include <iomanip>

#define OUTPUT_PRECISION 15

StdoutCsvObserver::StdoutCsvObserver(const std::vector<ObserveTarget> &targets)
    : targets(targets) {
  // nothing to do
}

StdoutCsvObserver::StdoutCsvObserver(const StdoutCsvObserver &observer)
    : targets(observer.targets) {
  // nothing to do
}

StdoutCsvObserver::~StdoutCsvObserver() {
  this->targets.clear();
}

void StdoutCsvObserver::operator()(const SBMLSystem::state &x, double t) {
  outputState(x, t);
}

void StdoutCsvObserver::outputState(const SBMLSystem::state &x, double t) {
  std::cout << t;
  std::cout << std::setprecision(OUTPUT_PRECISION);
  for (auto target : this->targets) {
    auto index = target.getStateIndex();
    std::cout << "," << x[index];
  }
  std::cout << std::endl;
}

void StdoutCsvObserver::outputHeader() {
  std::cout << "time";
  for (auto target : this->targets) {
    std::cout << "," << target.getId();
  }
  std::cout << std::endl;
}