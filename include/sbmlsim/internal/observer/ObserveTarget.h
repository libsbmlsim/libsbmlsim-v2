#ifndef INCLUDE_SBMLSIM_INTERNAL_OBSERVER_OBSERVETARGET_H_
#define INCLUDE_SBMLSIM_INTERNAL_OBSERVER_OBSERVETARGET_H_

#include <string>

class ObserveTarget {
 public:
  ObserveTarget(const std::string &id, unsigned int stateIndex);
  ObserveTarget(const ObserveTarget &observeTarget);
  ~ObserveTarget();
  const std::string &getId() const;
  unsigned int getStateIndex() const;
 private:
  const std::string id;
  const unsigned int stateIndex;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_OBSERVER_OBSERVETARGET_H_ */
