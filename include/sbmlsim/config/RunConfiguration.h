#ifndef INCLUDE_SBMLSIM_CONFIG_RUNCONFIGURATION_H_
#define INCLUDE_SBMLSIM_CONFIG_RUNCONFIGURATION_H_

#include <string>
#include <vector>
#include "sbmlsim/config/OutputField.h"

class RunConfiguration {
 public:
  RunConfiguration(double duration, double stepInterval, std::vector<OutputField> outputFields,
                   double absoluteTolerance = 1e-7, double relativeTolerance = 1e-4);
  RunConfiguration(double start, double duration, double stepInterval, std::vector<OutputField> outputFields,
                   double absoluteTolerance = 1e-7, double relativeTolerance = 1e-4);
  ~RunConfiguration();
  double getStart() const;
  double getDuration() const;
  double getStepInterval() const;
  const std::vector<OutputField> &getOutputFields() const;
  double getAbsoluteTolerance() const;
  double getRelativeTolerance() const;
 private:
  const double start;
  const double duration;
  const double stepInterval;
  const std::vector<OutputField> outputFields;
  const double absoluteTolerance;
  const double relativeTolerance;
};

#endif /* INCLUDE_SBMLSIM_CONFIG_RUNCONFIGURATION_H_ */
