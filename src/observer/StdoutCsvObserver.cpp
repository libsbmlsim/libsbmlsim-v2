#include "sbmlsim/observer/StdoutCsvObserver.h"

void StdoutCsvObserver::operator()(const SBMLSystem::state &x, double t) {
  std::cout << t;
  for (int i = 0; i < x.size(); i++) {
    std::cout << "," << x[i];
  }
  std::cout << std::endl;
}
