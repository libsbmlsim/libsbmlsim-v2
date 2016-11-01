#include "sbmlsim/state/ParameterStateFactory.h"
#include "sbmlsim/state/ParameterState.h"
#include "sbmlsim/def.h"

ParameterState ParameterStateFactory::createFromSBMLModel(const Model *model) {
  GlobalParameterMap globalParameters;
  LocalParameterMap localParameters;

  unsigned int numGlobalParameters = model->getNumParameters();
  for (unsigned int i = 0; i < numGlobalParameters; i++) {
    const Parameter *parameter = model->getParameter(i);
    const ParameterId parameterId = parameter->getId();
    double parameterValue = parameter->getValue();
    globalParameters[parameterId] = parameterValue;
  }

  unsigned int numReactions = model->getNumReactions();
  for (unsigned int i = 0; i < numReactions; i++) {
    const Reaction *reaction = model->getReaction(i);
    const ReactionId reactionId = reaction->getId();
    localParameters[reactionId] = ParameterMap();

    const KineticLaw *kineticLaw = reaction->getKineticLaw();
    unsigned int numLocalParamters = kineticLaw->getNumLocalParameters();
    for (unsigned int j = 0; j < numLocalParamters; j++) {
      const Parameter *parameter = kineticLaw->getLocalParameter(j);
      const ParameterId parameterId = parameter->getId();
      double parameterValue = parameter->getValue();
      localParameters[reactionId][parameterId] = parameterValue;
    }
  }

  return ParameterState(globalParameters, localParameters);
}
