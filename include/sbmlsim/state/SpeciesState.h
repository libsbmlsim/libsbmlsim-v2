#ifndef INCLUDE_SBMLSIM_STATE_SPECIESSTATE_H_
#define INCLUDE_SBMLSIM_STATE_SPECIESSTATE_H_

#include <string>
#include <map>
#include "sbmlsim/def.h"

struct SpeciesState {
 public:
  SpeciesState();
  SpeciesState(const SpeciesMap &speciesRawState);
  virtual ~SpeciesState();
  double getSpeciesValue(const SpeciesId &speciesId) const;
  void setSpeciesValue(const SpeciesId &speciesId, double value);
  bool hasSpeciesValue(const SpeciesId &speciesId) const;

 private:
  SpeciesMap speciesRawState;
};

#endif /* INCLUDE_SBMLSIM_STATE_SPECIESSTATE_H_ */
