#include "sbmlsim/internal/wrapper/ParameterWrapper.h"

ParameterWrapper::ParameterWrapper(const Parameter *parameter) {
  this->id = parameter->getId();
  this->reactionId = "";
  this->value = parameter->getValue();
  this->scope = ParameterScope::GLOBAL;
}

ParameterWrapper::ParameterWrapper(const Parameter *parameter, const Reaction *reaction) {
  this->id = parameter->getId();
  this->reactionId = reaction->getId();
  this->value = parameter->getValue();
  this->scope = ParameterScope::LOCAL;
}

ParameterWrapper::ParameterWrapper(const ParameterWrapper &parameter) {
  this->id = parameter.id;
  this->reactionId = parameter.reactionId;
  this->value = parameter.value;
  this->scope = parameter.scope;
}

ParameterWrapper::~ParameterWrapper() {
  // nothing to do
}

ParameterWrapper *ParameterWrapper::asGlobalParameter(const Parameter *parameter) {
  return new ParameterWrapper(parameter);
}

ParameterWrapper *ParameterWrapper::asLocalParameter(const Parameter *parameter, const Reaction *reaction) {
  return new ParameterWrapper(parameter, reaction);
}

const std::string &ParameterWrapper::getId() const {
  return this->id;
}

const std::string &ParameterWrapper::getReactionId() const {
  return this->reactionId;
}

void ParameterWrapper::setValue(double value) {
  this->value = value;
}

double ParameterWrapper::getValue() const {
  return this->value;
}

bool ParameterWrapper::isGlobalParameter() const {
  return this->scope == ParameterScope::GLOBAL;
}

bool ParameterWrapper::isLocalParameter() const {
  return this->scope == ParameterScope::LOCAL;
}
