#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_RATERULEWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_RATERULEWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

class RateRuleWrapper {
 public:
  explicit RateRuleWrapper(const RateRule *rateRule);
  RateRuleWrapper(const RateRuleWrapper &rateRule);
  ~RateRuleWrapper();
  const std::string &getVariable() const;
  const ASTNode *getMath() const;
 private:
  std::string variable;
  ASTNode *math;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_RATERULEWRAPPER_H_ */
