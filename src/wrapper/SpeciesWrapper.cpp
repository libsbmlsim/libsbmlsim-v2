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

  this->substanceUnitsOnly = species->getHasOnlySubstanceUnits();
  if (!this->substanceUnitsOnly &&
      (!compartment->isSetSpatialDimensions() || compartment->getSpatialDimensions() > 0)) {
    this->divideByCompartmentSize = true;
  } else {
    this->divideByCompartmentSize = false;
  }
}

SpeciesWrapper::SpeciesWrapper(const SpeciesWrapper &species) {
  this->id = species.id;
  this->initialAmountValue = species.initialAmountValue;
  this->amountValue = species.amountValue;
  this->compartmentId = species.compartmentId;
  this->substanceUnitsOnly = species.substanceUnitsOnly;
  this->boundaryCondition = species.boundaryCondition;
  this->constant = species.constant;
  this->divideByCompartmentSize = species.divideByCompartmentSize;
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

bool SpeciesWrapper::hasOnlySubstanceUnits() const {
  return this->substanceUnitsOnly;
}

bool SpeciesWrapper::hasBoundaryCondition() const {
  return this->boundaryCondition;
}

bool SpeciesWrapper::isConstant() const {
  return this->constant;
}

bool SpeciesWrapper::shouldDivideByCompartmentSize() const {
  return this->divideByCompartmentSize;
}
