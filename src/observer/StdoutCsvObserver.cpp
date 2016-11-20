#include "sbmlsim/internal/observer/StdoutCsvObserver.h"
#include <iomanip>

void StdoutCsvObserver::operator()(const SBMLSystem::state &x, double t) {
  std::cout << t;
  for (int i = 0; i < x.size(); i++) {
    std::cout << "," << std::setprecision(15) << x[i];
  }
  std::cout << std::endl;
}
