/**
 * @file RunConfiguration.cpp
 * @author Akito Tabira
 * @date 2016/10/26
 */
#include "sbmlsim/RunConfiguration.h"

RunConfiguration::RunConfiguration(double duration, double stepInterval)
  : duration(duration), stepInterval(stepInterval) {
  // nothing to do
}

RunConfiguration::~RunConfiguration() {
  // nothing to do
}

double RunConfiguration::getDuration() const {
  return duration;
}

double RunConfiguration::getStepInterval() const {
  return stepInterval;
}

double RunConfiguration::getDt() const {
  return this->getStepInterval();
}
