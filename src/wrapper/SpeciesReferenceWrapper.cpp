#include "sbmlsim/internal/wrapper/SpeciesReferenceWrapper.h"

SpeciesReferenceWrapper::SpeciesReferenceWrapper(const SpeciesReference *speciesReference) {
  this->speciesId = speciesReference->getSpecies();
  if (speciesReference->isSetStoichiometry()) {
    this->stoichiometry = speciesReference->getStoichiometry();
  } else {
    this->stoichiometry = 1.0;
  }
}

SpeciesReferenceWrapper::SpeciesReferenceWrapper(const SpeciesReferenceWrapper &speciesReference) {
  this->speciesId = speciesReference.speciesId;
  this->stoichiometry = speciesReference.stoichiometry;
}

SpeciesReferenceWrapper::~SpeciesReferenceWrapper() {
  // nothing to do
}

const std::string &SpeciesReferenceWrapper::getSpeciesId() const {
  return this->speciesId;
}

double SpeciesReferenceWrapper::getStoichiometry() const {
  return this->stoichiometry;
}
