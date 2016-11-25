#include "sbmlsim/internal/wrapper/EventWrapper.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"

EventWrapper::EventWrapper(const Event *event) {
  this->triggerState = false;

  this->trigger = ASTNodeUtil::rewriteFunctionDefinition(
      event->getTrigger()->getMath(),
      event->getModel()->getListOfFunctionDefinitions());

  for (auto i = 0; i < event->getNumEventAssignments(); i++) {
    auto eventAssignment = event->getEventAssignment(i);
    this->eventAssignments.push_back(EventAssignmentWrapper(eventAssignment));
  }
}

EventWrapper::EventWrapper(const EventWrapper &event) {
  this->trigger = event.trigger->deepCopy();
  this->eventAssignments = event.eventAssignments;
}

EventWrapper::~EventWrapper() {
  delete this->trigger;
  this->eventAssignments.clear();
}

const ASTNode *EventWrapper::getTrigger() const {
  return this->trigger;
}

const std::vector<EventAssignmentWrapper> &EventWrapper::getEventAssignments() const {
  return this->eventAssignments;
}

void EventWrapper::setTriggerState(bool triggerState) {
  this->triggerState = triggerState;
}

bool EventWrapper::getTriggerState() const {
  return this->triggerState;
}
