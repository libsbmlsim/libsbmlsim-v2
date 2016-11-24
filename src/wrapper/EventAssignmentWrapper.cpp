#include "sbmlsim/internal/wrapper/EventAssignmentWrapper.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"

EventAssignmentWrapper::EventAssignmentWrapper(const EventAssignment *eventAssignment) {
  this->variable = eventAssignment->getVariable();
  this->math = ASTNodeUtil::rewriteFunctionDefinition(
      eventAssignment->getMath(),
      eventAssignment->getModel()->getListOfFunctionDefinitions());
}

EventAssignmentWrapper::EventAssignmentWrapper(const EventAssignmentWrapper &eventAssignment) {
  this->variable = eventAssignment.variable;
  this->math = eventAssignment.math->deepCopy();
}

EventAssignmentWrapper::~EventAssignmentWrapper() {
  delete this->math;
}

const std::string &EventAssignmentWrapper::getVariable() const {
  return this->variable;
}

const ASTNode *EventAssignmentWrapper::getMath() const {
  return this->math;
}
