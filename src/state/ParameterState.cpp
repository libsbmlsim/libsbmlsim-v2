#include "sbmlsim/state/ParameterState.h"

ParameterState::ParameterState(const GlobalParameterMap &globalParameters,
      const LocalParameterMap &localParameters) {
  this->globalParameters = GlobalParameterMap(globalParameters);
  this->localParameters = LocalParameterMap(localParameters);
}

ParameterState::~ParameterState() {
  this->globalParameters.clear();
  this->localParameters.clear();
}

double ParameterState::getGlobalParameterValue(const ParameterId &parameterId) const {
  return this->globalParameters.at(parameterId);
}

void ParameterState::setGlobalParameterValue(const ParameterId &parameterId, double value) {
  this->globalParameters[parameterId] = value;
}

double ParameterState::getLocalParameterValue(const ParameterId &parameterId, const ReactionId &reactionId) const {
  return this->localParameters.at(reactionId).at(parameterId);
}

double ParameterState::getParameterValue(const ParameterId &parameterId, const ReactionId &reactionId) const {
  if (this->hasLocalParameter(parameterId, reactionId)) {
    return this->getLocalParameterValue(parameterId, reactionId);
  } else {
    return this->getGlobalParameterValue(parameterId);
  }
}

bool ParameterState::hasGlobalParameter(const ParameterId &parameterId) const {
  if (this->globalParameters.find(parameterId) != this->globalParameters.end()) {
    return true;
  } else {
    return false;
  }
}

bool ParameterState::hasLocalParameter(const ParameterId &parameterId, const ReactionId &reactionId) const {
  if (this->localParameters.at(reactionId).find(parameterId) != this->localParameters.at(reactionId).end()) {
    return true;
  } else {
    return false;
  }
}

bool ParameterState::hasParameter(const ParameterId &parameterId, const ReactionId &reactionId) const {
  return this->hasGlobalParameter(parameterId) || this->hasLocalParameter(parameterId, reactionId);
}
