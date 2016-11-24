#include "sbmlsim/internal/wrapper/ParameterWrapper.h"

ParameterWrapper::ParameterWrapper(const Parameter *parameter)
    : id(parameter->getId()), value(parameter->getValue()) {
  // nothing to do
}

ParameterWrapper::ParameterWrapper(const ParameterWrapper &parameter)
    : id(parameter.id), value(parameter.value) {
  // nothing to do
}

ParameterWrapper::~ParameterWrapper() {
  // nothing to do
}

const std::string &ParameterWrapper::getId() const {
  return this->id;
}

double ParameterWrapper::getValue() const {
  return this->value;
}
