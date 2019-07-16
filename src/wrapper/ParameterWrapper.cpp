#include "sbmlsim/internal/wrapper/ParameterWrapper.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"

ParameterWrapper::ParameterWrapper(const Parameter *parameter)
    : id(parameter->getId()), value(parameter->getValue()) {
  // for PI
  this->id = parameter->getId();
  ASTNode *val;
  val->setId(this->getId());
  val->setValue(this->getValue());
  this->value = ASTNodeUtil::rewriteFamousConstants(val);
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
