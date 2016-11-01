#ifndef INCLUDE_SBMLSIM_SBMLSIM_H_
#define INCLUDE_SBMLSIM_SBMLSIM_H_

#include <sbml/SBMLTypes.h>
#include <string>
#include "sbmlsim/RunConfiguration.h"

class SBMLSim {
 public:
  static void simulate(const std::string &filepath, const RunConfiguration &conf);
  static void simulate(const SBMLDocument *document, const RunConfiguration &conf);
 private:
  SBMLSim() {}
  ~SBMLSim() {}
  static void simulate(const Model *model, const RunConfiguration &conf);
};

#endif /* INCLUDE_SBMLSIM_SBMLSIM_H_ */
