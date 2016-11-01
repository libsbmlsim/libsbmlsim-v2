#ifndef INCLUDE_SBMLSIM_OBSERVER_IOBSERVER_H_
#define INCLUDE_SBMLSIM_OBSERVER_IOBSERVER_H_

class IObserver {
 public:
  virtual IObserver();
  virtual ~IObserver();
  virtual void operator()();
};

#endif /* INCLUDE_SBMLSIM_OBSERVER_IOBSERVER_H_ */
