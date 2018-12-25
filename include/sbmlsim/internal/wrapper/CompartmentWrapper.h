#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_COMPARTMENTWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_COMPARTMENTWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

LIBSBML_CPP_NAMESPACE_USE

class CompartmentWrapper {
 public:
  explicit CompartmentWrapper(const Compartment *compartment);
  CompartmentWrapper(const CompartmentWrapper &compartment);
  ~CompartmentWrapper();
  const std::string &getId() const;
  double getValue() const;
 private:
  std::string id;
  double value;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_COMPARTMENTWRAPPER_H_ */
