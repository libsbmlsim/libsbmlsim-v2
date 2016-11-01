#ifndef INCLUDE_SBMLSIM_SOLVER_ISOLVER_H_
#define INCLUDE_SBMLSIM_SOLVER_ISOLVER_H_

class ISolver {
 public:
  virtual ~ISolver();
  virtual void solve();
 private:
};

#endif /* INCLUDE_SBMLSIM_SOLVER_ISOLVER_H_ */
