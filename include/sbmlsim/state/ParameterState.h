#ifndef INCLUDE_SBMLSIM_STATE_PARAMETERSTATE_H_
#define INCLUDE_SBMLSIM_STATE_PARAMETERSTATE_H_

#include <sbml/SBMLTypes.h>
#include <string>
#include <map>
#include "sbmlsim/def.h"

struct ParameterState {
 public:
  ParameterState(const GlobalParameterMap &globalParameters,
      const LocalParameterMap &localParameters);
  virtual ~ParameterState();
  double getGlobalParameterValue(const ParameterId &parameterId) const;
  void setGlobalParameterValue(const ParameterId &parameterId, double value);
  double getLocalParameterValue(const ParameterId &parameterId, const ReactionId &reactionId) const;
  double getParameterValue(const ParameterId &parameterId, const ReactionId &reactionId) const;
  bool hasGlobalParameter(const ParameterId &parameterId) const;
  bool hasLocalParameter(const ParameterId &parameterId, const ReactionId &reactionId) const;
  bool hasParameter(const ParameterId &parameterId, const ReactionId &reactionId) const;

 private:
  ParameterMap globalParameters;
  LocalParameterMap localParameters;
};

#endif /* INCLUDE_SBMLSIM_STATE_PARAMETERSTATE_H_ */
