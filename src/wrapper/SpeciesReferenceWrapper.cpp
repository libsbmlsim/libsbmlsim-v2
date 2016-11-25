#include "sbmlsim/internal/wrapper/SpeciesReferenceWrapper.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"

SpeciesReferenceWrapper::SpeciesReferenceWrapper(const SpeciesReference *speciesReference) {
  this->speciesId = speciesReference->getSpecies();
  if (speciesReference->isSetStoichiometryMath()) {
    this->stoichiometryMath = ASTNodeUtil::rewriteFunctionDefinition(
        speciesReference->getStoichiometryMath()->getMath(),
        speciesReference->getModel()->getListOfFunctionDefinitions());
    this->stoichiometryType = StoichiometryType::MATH;
  } else if (speciesReference->isSetStoichiometry()) {
    this->stoichiometry = speciesReference->getStoichiometry();
    this->stoichiometryType = StoichiometryType::VALUE;
    this->stoichiometryMath = NULL;
  } else {
    this->stoichiometry = 1.0;
    this->stoichiometryType = StoichiometryType::VALUE;
    this->stoichiometryMath = NULL;
  }
}

SpeciesReferenceWrapper::SpeciesReferenceWrapper(const SpeciesReferenceWrapper &speciesReference) {
  this->speciesId = speciesReference.speciesId;
  this->stoichiometry = speciesReference.stoichiometry;
  if (speciesReference.stoichiometryType == StoichiometryType::MATH) {
    this->stoichiometryMath = speciesReference.stoichiometryMath->deepCopy();
  } else {
    this->stoichiometryMath = NULL;
  }
  this->stoichiometryType = speciesReference.stoichiometryType;
}

SpeciesReferenceWrapper::~SpeciesReferenceWrapper() {
  if (this->hasStoichiometryMath() && this->stoichiometryMath != NULL) {
    delete this->stoichiometryMath;
  }
}

const std::string &SpeciesReferenceWrapper::getSpeciesId() const {
  return this->speciesId;
}

double SpeciesReferenceWrapper::getStoichiometry() const {
  return this->stoichiometry;
}

const ASTNode *SpeciesReferenceWrapper::getStoichiometryMath() const {
  return this->stoichiometryMath;
}

const StoichiometryType &SpeciesReferenceWrapper::getStoichiometryType() const {
  return this->stoichiometryType;
}

bool SpeciesReferenceWrapper::hasStoichiometryMath() const {
  return this->stoichiometryType == StoichiometryType::MATH;
}
