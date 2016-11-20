#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_EVENTASSIGNMENTWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_EVENTASSIGNMENTWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

class EventAssignmentWrapper {
public:
  explicit EventAssignmentWrapper(const EventAssignment *eventAssignment);
  EventAssignmentWrapper(const EventAssignmentWrapper &eventAssignment);
  ~EventAssignmentWrapper();
  const std::string &getVariable() const;
  const ASTNode *getMath() const;
private:
  std::string variable;
  const ASTNode *math;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_EVENTASSIGNMENTWRAPPER_H_ */
