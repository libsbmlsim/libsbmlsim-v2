#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_PARAMETERWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_PARAMETERWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

class ParameterWrapper { // global parameter only
 public:
  explicit ParameterWrapper(const Parameter *parameter);
  ParameterWrapper(const ParameterWrapper &parameter);
  ~ParameterWrapper();
  const std::string &getId() const;
  double getValue() const;
 private:
  const std::string id;
  const double value;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_PARAMETERWRAPPER_H_ */
