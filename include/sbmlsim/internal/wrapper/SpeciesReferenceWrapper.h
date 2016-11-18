#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESREFERENCEWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESREFERENCEWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>

class SpeciesReferenceWrapper {
 public:
  explicit SpeciesReferenceWrapper(const SpeciesReference *speciesReference);
  SpeciesReferenceWrapper(const SpeciesReferenceWrapper &speciesReference);
  ~SpeciesReferenceWrapper();
  const std::string &getSpeciesId() const;
  double getStoichiometry() const;
 private:
  std::string speciesId;
  double stoichiometry;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_SPECIESREFERENCEWRAPPER_H_ */
