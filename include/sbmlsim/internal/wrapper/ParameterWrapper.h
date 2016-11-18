#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_PARAMETERWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_PARAMETERWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

enum class ParameterScope;

class ParameterWrapper {
 public:
  static ParameterWrapper asGlobalParameter(const Parameter *parameter);
  static ParameterWrapper asLocalParameter(const Parameter *parameter, const Reaction *reaction);
  ParameterWrapper(const ParameterWrapper &parameter);
  ~ParameterWrapper();
  const std::string &getId() const;
  const std::string &getReactionId() const;
  void setValue(double value);
  double getValue() const;
  bool isGlobalParameter() const;
  bool isLocalParameter() const;
 private:
  explicit ParameterWrapper(const Parameter *parameter);
  ParameterWrapper(const Parameter *parameter, const Reaction *reaction);
 private:
  std::string id;
  std::string reactionId;
  double value;
  ParameterScope scope;
};

enum class ParameterScope {
  GLOBAL,
  LOCAL
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_PARAMETERWRAPPER_H_ */
