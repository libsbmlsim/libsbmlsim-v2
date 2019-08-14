#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESREFERENCEWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESREFERENCEWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

LIBSBML_CPP_NAMESPACE_USE

enum class StoichiometryType;

class SpeciesReferenceWrapper {
 public:
  explicit SpeciesReferenceWrapper(const SpeciesReference *speciesReference);
  SpeciesReferenceWrapper(const SpeciesReferenceWrapper &speciesReference);
  ~SpeciesReferenceWrapper();
  const std::string &getSpeciesId() const;
  double getStoichiometry() const;
  const ASTNode *getStoichiometryMath() const;
  const StoichiometryType &getStoichiometryType() const;
  bool hasStoichiometryMath() const;
  bool hasConversionFactorOrNot() const;
  double getConversionFactor() const;
 private:
  std::string speciesId;
  double stoichiometry;
  ASTNode *stoichiometryMath;
  StoichiometryType stoichiometryType;
  double conversionFactor;
  bool hasConversionFactor;
};

enum class StoichiometryType {
  VALUE,
  MATH
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESREFERENCEWRAPPER_H_ */
