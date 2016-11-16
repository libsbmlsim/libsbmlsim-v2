#include "sbmlsim/config/OutputField.h"

OutputField::OutputField(const std::string &id, const OutputType &type)
    : id(id), type(type) {
  // nothing to do
}

OutputField::~OutputField() {
  // nothing to do
}

const std::string &OutputField::getId() const {
  return this->id;
}

const OutputType &OutputField::getType() const {
  return this->type;
}
