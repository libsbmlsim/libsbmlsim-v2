#ifndef INCLUDE_SBMLSIM_SBMLSIM_H_
#define INCLUDE_SBMLSIM_SBMLSIM_H_

#include <sbml/SBMLTypes.h>
#include <string>
#include "sbmlsim/config/RunConfiguration.h"
#include "sbmlsim/internal/wrapper/ModelWrapper.h"

class SBMLSim {
 public:
  static void simulate(const std::string &filepath, const RunConfiguration &conf);
  static void simulate(const SBMLDocument *document, const RunConfiguration &conf);
 private:
  SBMLSim() {}
  ~SBMLSim() {}
  static void simulate(const Model *model, const RunConfiguration &conf);
  static void simulateRungeKutta4(const ModelWrapper *model, const RunConfiguration &conf);
  static void simulateRungeKuttaDopri5(const ModelWrapper *model, const RunConfiguration &conf);
  static void simulateRungeKuttaFehlberg78(const ModelWrapper *model, const RunConfiguration &conf);
  static void simulateRosenbrock4(const ModelWrapper *model, const RunConfiguration &conf);
};

#endif /* INCLUDE_SBMLSIM_SBMLSIM_H_ */
