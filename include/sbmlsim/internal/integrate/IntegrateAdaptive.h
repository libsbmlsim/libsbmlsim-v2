#ifndef INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATEADAPTIVE_H_
#define INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATEADAPTIVE_H_

#include <boost/numeric/odeint.hpp>
#include "sbmlsim/internal/system/SBMLSystem.h"

using namespace boost::numeric;

namespace sbmlsim {

template<class Stepper, class Observer>
size_t integrate_adaptive_detail(
    Stepper stepper, SBMLSystem system, SBMLSystem::state &start_state,
    double &start_time, double end_time, double &dt,
    Observer observer, odeint::controlled_stepper_tag) {
  typename odeint::unwrap_reference<Observer>::type &obs = observer;
  typename odeint::unwrap_reference<Stepper>::type &st = stepper;

  odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails
  size_t count = 0;
  while (odeint::detail::less_with_sign(start_time, end_time, dt)) {
    obs(start_state, start_time);
    if (odeint::detail::less_with_sign(end_time, start_time + dt, dt)) {
      dt = end_time - start_time;
    }

    odeint::controlled_step_result res;
    do {
      res = st.try_step(system, start_state, start_time, dt);
      fail_checker();  // check number of failed steps
    } while (res == odeint::fail);
    fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker

    ++count;
  }
  obs(start_state, start_time);
  return count;
}

} /* namespace sbmlsim */

#endif /* INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATEADAPTIVE_H_ */