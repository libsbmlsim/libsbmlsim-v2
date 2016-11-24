#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

enum class SpeciesValueType;

class SpeciesWrapper {
 public:
  explicit SpeciesWrapper(const Species *species);
  SpeciesWrapper(const SpeciesWrapper &species);
  ~SpeciesWrapper();
  const std::string &getId() const;
  double getAmountValue() const;
  double getInitialAmountValue() const;
  const std::string &getCompartmentId() const;
  bool hasOnlySubstanceUnits() const;
  bool hasBoundaryCondition() const;
  bool isConstant() const;
  bool shouldDivideByCompartmentSize() const;
 private:
  std::string id;
  double amountValue;
  double initialAmountValue;
  std::string compartmentId;
  bool substanceUnitsOnly;
  bool boundaryCondition;
  bool constant;
  bool divideByCompartmentSize;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESWRAPPER_H_ */
