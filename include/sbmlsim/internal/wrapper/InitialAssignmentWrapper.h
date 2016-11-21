#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_INITIALASSIGNMENTWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_INITIALASSIGNMENTWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

class InitialAssignmentWrapper {
public:
  explicit InitialAssignmentWrapper(const InitialAssignment *initialAssignment);
  InitialAssignmentWrapper(const InitialAssignmentWrapper &initialAssignment);
  ~InitialAssignmentWrapper();
  const std::string &getSymbol() const;
  const ASTNode *getMath() const;
private:
  std::string symbol;
  ASTNode *math;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_INITIALASSIGNMENTWRAPPER_H_ */
