#include "sbmlsim/state/SpeciesState.h"

SpeciesState::SpeciesState() {
  this->speciesRawState = SpeciesMap();
}

SpeciesState::SpeciesState(const SpeciesMap &speciesRawState) {
  this->speciesRawState = SpeciesMap(speciesRawState);
}

SpeciesState::~SpeciesState() {
  this->speciesRawState.clear();
}

double SpeciesState::getSpeciesValue(const SpeciesId &speciesId) const {
  return this->speciesRawState.at(speciesId);
}

void SpeciesState::setSpeciesValue(const SpeciesId &speciesId, double value) {
  this->speciesRawState[speciesId] = value;
}

bool SpeciesState::hasSpeciesValue(const SpeciesId &speciesId) const {
  if (this->speciesRawState.find(speciesId) != this->speciesRawState.end()) {
    return true;
  } else {
    return false;
  }
}
