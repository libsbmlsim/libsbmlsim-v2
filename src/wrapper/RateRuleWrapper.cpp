#include "sbmlsim/internal/wrapper/RateRuleWrapper.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"

RateRuleWrapper::RateRuleWrapper(const RateRule *rateRule) {
  this->variable = rateRule->getVariable();
  this->math = ASTNodeUtil::rewriteFunctionDefinition(
      rateRule->getMath(),
      rateRule->getModel()->getListOfFunctionDefinitions());
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
