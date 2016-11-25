#include "sbmlsim/internal/observer/ObserveTarget.h"

ObserveTarget::ObserveTarget(const std::string &id, unsigned int stateIndex)
    : id(id), stateIndex(stateIndex) {
  // nothing to do
}

ObserveTarget::ObserveTarget(const ObserveTarget &observeTarget)
    : id(observeTarget.id), stateIndex(observeTarget.stateIndex) {
  // nothing to do
}

ObserveTarget::~ObserveTarget() {
  // nothing to do
}

const std::string &ObserveTarget::getId() const {
  return this->id;
}

unsigned int ObserveTarget::getStateIndex() const {
  return this->stateIndex;
}
