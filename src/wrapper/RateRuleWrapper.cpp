#include "sbmlsim/internal/wrapper/RateRuleWrapper.h"

RateRuleWrapper::RateRuleWrapper(const RateRule *rateRule) {
  this->variable = rateRule->getVariable();
  this->math = rateRule->getMath()->deepCopy();
}

RateRuleWrapper::RateRuleWrapper(const RateRuleWrapper &rateRule) {
  this->variable = rateRule.variable;
  this->math = rateRule.math->deepCopy();
}

RateRuleWrapper::~RateRuleWrapper() {
  delete this->math;
}

const std::string &RateRuleWrapper::getVariable() const {
  return this->variable;
}

const ASTNode *RateRuleWrapper::getMath() const {
  return this->math;
}
