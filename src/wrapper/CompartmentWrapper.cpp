#include "sbmlsim/internal/wrapper/CompartmentWrapper.h"

CompartmentWrapper::CompartmentWrapper(const Compartment *compartment) {
  this->id = compartment->getId();
  if (compartment->isSetSize()) {
    this->value = compartment->getSize();
  } else {
    this->value = 1.0;
  }
}

CompartmentWrapper::CompartmentWrapper(const CompartmentWrapper &compartment) {
  this->id = compartment.id;
  this->value = compartment.value;
}

CompartmentWrapper::~CompartmentWrapper() {
  // nothing to do
}

const std::string &CompartmentWrapper::getId() const {
  return this->id;
}

double CompartmentWrapper::getValue() const {
  return this->value;
}
