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

  size_t count = 0;
  while (odeint::detail::less_with_sign(start_time, end_time, dt)) {
    obs(start_state, start_time);
    if (odeint::detail::less_with_sign(end_time, start_time + dt, dt)) {
      dt = end_time - start_time;
    }

    const int MAX_STEPS = 500;
    int steps = 0;
    odeint::controlled_step_result res;
    do {
      res = st.try_step(system, start_state, start_time, dt);

      // DO NOT USE failed_step_checker to make it to compatible with boost-1.54.0.
      steps++;
      if (steps >= MAX_STEPS) {
        char errorMsg[200];
        sprintf(errorMsg, "Max number of iterations exceeded (%d).", MAX_STEPS);
        BOOST_THROW_EXCEPTION(odeint::no_progress_error(errorMsg));
      }
    } while (res == odeint::fail);

    ++count;
  }
  obs(start_state, start_time);
  return count;
}

} /* namespace sbmlsim */

#endif /* INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATEADAPTIVE_H_ */