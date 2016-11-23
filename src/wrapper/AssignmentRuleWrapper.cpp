#include "sbmlsim/internal/wrapper/AssignmentRuleWrapper.h"

AssignmentRuleWrapper::AssignmentRuleWrapper(const AssignmentRule *assignmentRule) {
  this->variable = assignmentRule->getVariable();
  this->math = assignmentRule->getMath()->deepCopy();
}

AssignmentRuleWrapper::AssignmentRuleWrapper(const AssignmentRuleWrapper &assignmentRule) {
  this->variable = assignmentRule.variable;
  this->math = assignmentRule.math->deepCopy();
}

AssignmentRuleWrapper::~AssignmentRuleWrapper() {
  delete this->math;
}

const std::string &AssignmentRuleWrapper::getVariable() const {
  return this->variable;
}

const ASTNode *AssignmentRuleWrapper::getMath() const {
  return this->math;
}
