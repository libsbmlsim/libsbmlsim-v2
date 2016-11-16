#include "sbmlsim/config/RunConfiguration.h"

RunConfiguration::RunConfiguration(double duration, double stepInterval, std::vector<OutputField> outputFields,
                                   double absoluteTolerance, double relativeTolerance)
    : start(0), duration(duration), stepInterval(stepInterval), outputFields(outputFields),
      absoluteTolerance(absoluteTolerance), relativeTolerance(relativeTolerance) {
  // nothing to do
}

RunConfiguration::RunConfiguration(double start, double duration, double stepInterval,
                                   std::vector<OutputField> outputFields, double absoluteTolerance,
                                   double relativeTolerance)
    : start(start), duration(duration), stepInterval(stepInterval), outputFields(outputFields),
      absoluteTolerance(absoluteTolerance), relativeTolerance(relativeTolerance) {
  // nothing to do
}

RunConfiguration::~RunConfiguration() {
  // nothing to do
}

double RunConfiguration::getStart() const {
  return this->start;
}

double RunConfiguration::getDuration() const {
  return this->duration;
}

double RunConfiguration::getStepInterval() const {
  return this->stepInterval;
}

const std::vector<OutputField> &RunConfiguration::getOutputFields() const {
  return this->outputFields;
}
