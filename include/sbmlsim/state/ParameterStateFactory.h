#ifndef INCLUDE_SBMLSIM_STATE_PARAMETERSTATEFACTORY_H_
#define INCLUDE_SBMLSIM_STATE_PARAMETERSTATEFACTORY_H_

#include <sbml/SBMLTypes.h>
#include "sbmlsim/state/ParameterState.h"

class ParameterStateFactory {
 public:
  static ParameterState createFromSBMLModel(const Model *model);
};

#endif /* INCLUDE_SBMLSIM_STATE_PARAMETERSTATEFACTORY_H_ */
