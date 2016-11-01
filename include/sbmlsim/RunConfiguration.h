#ifndef INCLUDE_SBMLSIM_RUNCONFIGURATION_H_
#define INCLUDE_SBMLSIM_RUNCONFIGURATION_H_

class RunConfiguration {
 public:
  RunConfiguration(double duration, double stepInterval);
  ~RunConfiguration();
  double getDuration() const;
  double getStepInterval() const;
  double getDt() const;
 private:
  const double duration;
  const double stepInterval;
};

#endif /* INCLUDE_SBMLSIM_RUNCONFIGURATION_H_ */
