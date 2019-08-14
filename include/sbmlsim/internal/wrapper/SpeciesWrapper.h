#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

LIBSBML_CPP_NAMESPACE_USE

class SpeciesWrapper {
 public:
  explicit SpeciesWrapper(const Species *species);
  SpeciesWrapper(const SpeciesWrapper &species);
  ~SpeciesWrapper();
  const std::string &getId() const;
  double getAmountValue() const;
  double getInitialAmountValue() const;
  const std::string &getCompartmentId() const;
  bool hasBoundaryCondition() const;
  bool isConstant() const;
  bool shouldDivideByCompartmentSizeOnEvaluation() const;
  bool shouldMultiplyByCompartmentSizeOnAssignment() const;
  bool hasConversionFactor() const;
  const std::string &getConversionFactor() const;
 private:
  std::string id;
  double amountValue;
  double initialAmountValue;
  std::string compartmentId;
  bool boundaryCondition;
  bool constant;
  bool divideByCompartmentSizeOnEvaluation;
  std::string conversionFactor;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESWRAPPER_H_ */
