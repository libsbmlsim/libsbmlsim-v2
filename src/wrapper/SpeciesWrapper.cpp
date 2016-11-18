#include "sbmlsim/internal/wrapper/SpeciesWrapper.h"

SpeciesWrapper::SpeciesWrapper(const Species *species) {
  this->id = species->getId();
  this->substanceUnitsOnly = species->getHasOnlySubstanceUnits();
  this->valueType = SpeciesValueType::UNKNOWN;

  if (species->isSetInitialAmount()) {
    this->valueType = SpeciesValueType::AMOUNT;
    this->initialValue = species->getInitialAmount();
  } else if (species->isSetInitialConcentration()) {
    this->valueType = SpeciesValueType::CONCENTRATION;
    this->initialValue = species->getInitialConcentration();
  } else {
    this->initialValue = 0.0;

    const Model *model = species->getModel();
    const std::string &compartmentId = species->getCompartment();
    const Compartment *compartment = model->getCompartment(compartmentId);
    unsigned int spatialDimensions = compartment->getSpatialDimensions();

    if (this->substanceUnitsOnly || spatialDimensions == 0) {
      this->valueType = SpeciesValueType::AMOUNT;
    } else {
      this->valueType = SpeciesValueType::CONCENTRATION;
    }
  }

  this->value = this->initialValue;
}

SpeciesWrapper::SpeciesWrapper(const SpeciesWrapper &species) {
  this->id = species.id;
  this->initialValue = species.initialValue;
  this->value = species.value;
  this->valueType = species.valueType;
  this->substanceUnitsOnly = species.substanceUnitsOnly;
}

SpeciesWrapper::~SpeciesWrapper() {
  // nothing to do
}

const std::string &SpeciesWrapper::getId() const {
  return this->id;
}

void SpeciesWrapper::setValue(double value) {
  this->value = value;
}

double SpeciesWrapper::getValue() const {
  return this->value;
}

double SpeciesWrapper::getInitialValue() const {
  return this->initialValue;
}

bool SpeciesWrapper::isAmountValue() const {
  return this->valueType == SpeciesValueType::AMOUNT;
}

bool SpeciesWrapper::isConcentrationValue() const {
  return this->valueType == SpeciesValueType::CONCENTRATION;
}

bool SpeciesWrapper::hasOnlySubstanceUnits() const {
  return this->substanceUnitsOnly;
}
