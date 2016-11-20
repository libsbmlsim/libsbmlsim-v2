#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_EVENTWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_EVENTWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <vector>
#include "sbmlsim/internal/wrapper/EventAssignmentWrapper.h"

class EventWrapper {
 public:
  explicit EventWrapper(const Event *event);
  EventWrapper(const EventWrapper &event);
  ~EventWrapper();
  const ASTNode *getTrigger() const;
  const std::vector<EventAssignmentWrapper> &getEventAssignments() const;
  void setTriggerState(bool triggerState);
  bool getTriggerState() const;
 private:
  ASTNode *trigger;
  std::vector<EventAssignmentWrapper> eventAssignments;
  bool triggerState;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_EVENTWRAPPER_H_ */
