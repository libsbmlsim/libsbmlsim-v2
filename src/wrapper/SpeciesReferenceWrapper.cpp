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

  if (speciesReference->getModel()->getSpecies(this->getSpeciesId())->isSetConversionFactor()) {
    this->hasConversionFactor = true;
    // Error here line 25 : 99% sure that it makes IntegationTest run forever
    this->conversionFactor = speciesReference->getModel()->getParameter(speciesReference->getModel()->getSpecies(this->getSpeciesId())->getConversionFactor())->getValue();
  } else {
    this->hasConversionFactor = false;
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
  this->conversionFactor = speciesReference.conversionFactor;
  this->hasConversionFactor = speciesReference.hasConversionFactor;
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

double SpeciesReferenceWrapper::getConversionFactor() const {
  return this->conversionFactor;
}

bool SpeciesReferenceWrapper::hasConversionFactorOrNot() const {
  return this->hasConversionFactor;
}