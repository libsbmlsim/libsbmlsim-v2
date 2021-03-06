#include "sbmlsim/internal/wrapper/AssignmentRuleWrapper.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"

AssignmentRuleWrapper::AssignmentRuleWrapper(const AssignmentRule *assignmentRule) {
  this->variable = assignmentRule->getVariable();
  auto maths = ASTNodeUtil::rewriteFunctionDefinition(
      assignmentRule->getMath(),
      assignmentRule->getModel()->getListOfFunctionDefinitions());
  this->math = ASTNodeUtil::rewriteFamousConstants(maths);
  delete maths;
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
