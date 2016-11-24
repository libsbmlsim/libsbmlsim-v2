#ifndef INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATECONST_H_
#define INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATECONST_H_

#include <boost/numeric/odeint.hpp>
#include "sbmlsim/internal/system/SBMLSystem.h"
#include "sbmlsim/internal/integrate/IntegrateAdaptive.h"

using namespace boost::numeric;

namespace sbmlsim {

template<class Stepper, class Observer>
size_t integrate_const_detail(
    Stepper stepper, SBMLSystem &system , SBMLSystem::state &start_state,
    double start_time, double end_time, double dt,
    Observer observer, odeint::stepper_tag) {
  typename odeint::unwrap_reference<Observer>::type &obs = observer;
  typename odeint::unwrap_reference<Stepper>::type &st = stepper;

  double time = start_time;
  const double time_step = dt;
  int step = 0;

  while (odeint::detail::less_eq_with_sign(time + time_step, end_time, dt)) {
    // assignment rules
    system.handleAssignmentRule(start_state, time);

    // initial assignments
    system.handleInitialAssignment(start_state, time);

    // observer
    obs(start_state, time);

    st.do_step(system, start_state, time, dt);

    // direct computation of the time avoids error propagation happening when using time += dt
    // we need clumsy type analysis to get boost units working here
    ++step;
    time = start_time + static_cast<typename odeint::unit_value_type<double>::type>(step) * dt;

    // event
    system.handleEvent(start_state, time);
  }

  // assignment rules
  system.handleAssignmentRule(start_state, time);

  // observer
  obs(start_state, time);

  return step;
}

template<class Stepper, class Observer>
size_t integrate_const_detail(
    Stepper &stepper, SBMLSystem &system, SBMLSystem::state &start_state,
    double start_time, double end_time, double dt,
    Observer observer, odeint::controlled_stepper_tag) {
  typename odeint::unwrap_reference<Observer>::type &obs = observer;

  double time = start_time;
  const double time_step = dt;
  int real_steps = 0;
  int step = 0;

  while (odeint::detail::less_eq_with_sign(time + time_step, end_time, dt)) {
    // assignment rules
    system.handleAssignmentRule(start_state, time);

    // initial assignments
    system.handleInitialAssignment(start_state, time);

    // observer
    obs(start_state, time);

    // integrate_adaptive_checked uses the given checker to throw if an overflow occurs
    real_steps += odeint::detail::integrate_adaptive(stepper, system, start_state, time,
                                                     time + time_step, dt,
                                                     odeint::null_observer(), odeint::controlled_stepper_tag());

    // direct computation of the time avoids error propagation happening when using time += dt
    // we need clumsy type analysis to get boost units working here
    step++;
    time = start_time + static_cast<typename odeint::unit_value_type<double>::type>(step) * time_step;

    // event
    system.handleEvent(start_state, time);
  }

  // assignment rules
  system.handleAssignmentRule(start_state, time);

  // observer
  obs(start_state, time);

  return real_steps;
}

template<class Stepper, class Time, class Observer>
size_t integrate_const(
    Stepper &stepper, SBMLSystem &system, SBMLSystem::state &start_state,
    Time start_time, Time end_time, Time dt, Observer observer) {
  typedef typename odeint::unwrap_reference<Stepper>::type::stepper_category stepper_category;
  return integrate_const_detail(stepper, system, start_state,
                                start_time, end_time, dt, observer, stepper_category());
}

} /* namespace sbmlsim */

#endif /* INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATECONST_H_ */
