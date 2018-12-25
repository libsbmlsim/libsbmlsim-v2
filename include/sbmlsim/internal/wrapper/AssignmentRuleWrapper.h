#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_ASSIGNMENTRULEWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_ASSIGNMENTRULEWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

LIBSBML_CPP_NAMESPACE_USE

class AssignmentRuleWrapper {
 public:
  explicit AssignmentRuleWrapper(const AssignmentRule *assignmentRule);
  AssignmentRuleWrapper(const AssignmentRuleWrapper &assignmentRule);
  ~AssignmentRuleWrapper();
  const std::string &getVariable() const;
  const ASTNode *getMath() const;
 private:
  std::string variable;
  ASTNode *math;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_ASSIGNMENTRULEWRAPPER_H_ */
