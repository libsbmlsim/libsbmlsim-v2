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
  void setValue(double value);
  double getValue() const;
  double getInitialValue() const;
  bool isAmountValue() const;
  bool isConcentrationValue() const;
  bool hasOnlySubstanceUnits() const;
  bool hasBoundaryCondition() const;
 private:
  std::string id;
  double value;
  double initialValue;
  SpeciesValueType valueType;
  bool substanceUnitsOnly;
  bool boundaryCondition;
};

enum class SpeciesValueType {
  UNKNOWN,
  AMOUNT,
  CONCENTRATION
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESWRAPPER_H_ */
