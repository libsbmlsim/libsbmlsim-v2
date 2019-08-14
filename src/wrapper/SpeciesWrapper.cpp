#include "sbmlsim/internal/wrapper/SpeciesWrapper.h"

SpeciesWrapper::SpeciesWrapper(const Species *species) {
  this->id = species->getId();

  const Model *model = species->getModel();
  const std::string &compartmentId = species->getCompartment();
  const Compartment *compartment = model->getCompartment(compartmentId);
  this->compartmentId = compartment->getId();

  if (species->isSetInitialAmount()) {
    this->initialAmountValue = species->getInitialAmount();
  } else if (species->isSetInitialConcentration()) {
    double compartmentSize = 1.0;
    if (compartment->isSetSize()) {
      compartmentSize = compartment->getSize();
    }
    this->initialAmountValue = species->getInitialConcentration() * compartmentSize;
  } else {
    this->initialAmountValue = 0.0;
  }

  this->amountValue = this->initialAmountValue;

  if (species->isSetBoundaryCondition()) {
    this->boundaryCondition = species->getBoundaryCondition();
  } else {
    this->boundaryCondition = false;
  }

  if (species->isSetConstant()) {
    this->constant = species->getConstant();
  } else {
    this->constant = false;
  }

  if (!species->getHasOnlySubstanceUnits() &&
      (!compartment->isSetSpatialDimensions() || compartment->getSpatialDimensions() > 0)) {
    this->divideByCompartmentSizeOnEvaluation = true;
  } else {
    this->divideByCompartmentSizeOnEvaluation = false;
  }

  if (species->isSetConversionFactor()) {
    this->hasConversionFactor = true;
    this->conversionFactor = model->getParameter(species->getConversionFactor())->getValue();
  } else {
    this->hasConversionFactor = false;
  }
}

SpeciesWrapper::SpeciesWrapper(const SpeciesWrapper &species) {
  this->id = species.id;
  this->initialAmountValue = species.initialAmountValue;
  this->amountValue = species.amountValue;
  this->compartmentId = species.compartmentId;
  this->boundaryCondition = species.boundaryCondition;
  this->constant = species.constant;
  this->divideByCompartmentSizeOnEvaluation = species.divideByCompartmentSizeOnEvaluation;
  this->conversionFactor = species.conversionFactor;
}

SpeciesWrapper::~SpeciesWrapper() {
  // nothing to do
}

const std::string &SpeciesWrapper::getId() const {
  return this->id;
}

double SpeciesWrapper::getAmountValue() const {
  return this->amountValue;
}

double SpeciesWrapper::getInitialAmountValue() const {
  return this->initialAmountValue;
}

const std::string &SpeciesWrapper::getCompartmentId() const {
  return this->compartmentId;
}

bool SpeciesWrapper::hasBoundaryCondition() const {
  return this->boundaryCondition;
}

bool SpeciesWrapper::isConstant() const {
  return this->constant;
}

bool SpeciesWrapper::shouldDivideByCompartmentSizeOnEvaluation() const {
  return this->divideByCompartmentSizeOnEvaluation;
}

bool SpeciesWrapper::shouldMultiplyByCompartmentSizeOnAssignment() const {
  return shouldDivideByCompartmentSizeOnEvaluation();
}

double SpeciesWrapper::conversionFactor() const {
  return this->conversionFactor;
}

bool SpeciesWrapper::hasConversionFactor() const {
  return this->hasConversionFactor;
}