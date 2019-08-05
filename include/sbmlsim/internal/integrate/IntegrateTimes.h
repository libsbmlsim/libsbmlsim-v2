/**
 * [Original]
 * Copyright 2011-2015 Mario Mulansky
 * Copyright 2012 Karsten Ahnert
 * Copyright 2012 Christoph Koke
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATETIMES_H_
#define INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATETIMES_H_

#include <boost/numeric/odeint.hpp>
#include "sbmlsim/internal/system/SBMLSystem.h"

using namespace boost::numeric;

namespace sbmlsim {

namespace internal {
/*
 * integrate_times for simple stepper
 */
template <class Stepper, class TimeIterator, class Time, class Observer>
size_t integrate_times(Stepper stepper, SBMLSystem &system, SBMLSystem::state &start_state, TimeIterator start_time,
                       TimeIterator end_time, Time dt, Observer observer, odeint::stepper_tag) {
  typedef typename odeint::unwrap_reference<Stepper>::type stepper_type;
  typedef typename odeint::unwrap_reference<Observer>::type observer_type;

  stepper_type &st = stepper;
  observer_type &obs = observer;
  typedef typename odeint::unit_value_type<Time>::type time_type;

  size_t steps = 0;
  Time current_dt = dt;
  while (true) {
    Time current_time = *start_time++;

    system.handleInitialAssignment(start_state, current_time);
    system.handleAssignmentRule(start_state, current_time);

    obs(start_state, current_time);
    if (start_time == end_time) break;
    while (odeint::detail::less_with_sign(current_time, static_cast<time_type>(*start_time), current_dt)) {
      system.handleAssignmentRule(start_state, current_time);

      current_dt = odeint::detail::min_abs(dt, *start_time - current_time);
      st.do_step(system, start_state, current_time, current_dt);

      system.handleEvent(start_state, current_time);

      current_time += current_dt;
      steps++;
    }
  }

  return steps;
}

/*
 * integrate_times for controlled stepper
 */
template <class Stepper, class TimeIterator, class Time, class Observer>
size_t integrate_times(Stepper stepper, SBMLSystem &system, SBMLSystem::state &start_state, TimeIterator start_time,
                       TimeIterator end_time, Time dt, Observer observer, odeint::controlled_stepper_tag) {
  typename odeint::unwrap_reference<Observer>::type &obs = observer;
  typename odeint::unwrap_reference<Stepper>::type &st = stepper;
  typedef typename odeint::unit_value_type<Time>::type time_type;

  odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails
  size_t steps = 0;
  while (true) {
    Time current_time = *start_time++;

    system.handleInitialAssignment(start_state, current_time);
    system.handleAssignmentRule(start_state, current_time);

    obs(start_state, current_time);
    if (start_time == end_time) break;
    while (odeint::detail::less_with_sign(current_time, static_cast<time_type>(*start_time), dt)) {
      system.handleAssignmentRule(start_state, current_time);

      // adjust stepsize to end up exactly at the observation point
      Time current_dt = odeint::detail::min_abs(dt, *start_time - current_time);
      if (st.try_step(system, start_state, current_time, current_dt) == odeint::success) {
        system.handleEvent(start_state, current_time);

        ++steps;
        // successful step -> reset the fail counter, see #173
        fail_checker.reset();
        // continue with the original step size if dt was reduced due to observation
        dt = odeint::detail::max_abs(dt, current_dt);
      } else {
        fail_checker();  // check for possible overflow of failed steps in step size adjustment
        dt = current_dt;
      }
    }
  }
  return steps;
}

} /* namespace internal */

/*
 * \brief Integrates while calling the observer at the time points given by sequence [times_start, time_end)
 * the two overloads are needed in order to solve the forwarding problem
 */
template <class Stepper, class TimeIterator, class Time, class Observer>
size_t integrate_times(Stepper stepper, SBMLSystem &system, SBMLSystem::state &start_state, TimeIterator times_start,
                       TimeIterator times_end, Time dt, Observer observer) {
  typedef typename odeint::unwrap_reference<Stepper>::type::stepper_category stepper_category;
  return internal::integrate_times(stepper, system, start_state, times_start, times_end, dt, observer,
                                   stepper_category());
}

} /* namespace sbmlsim */

#endif /* INCLUDE_SBMLSIM_INTERNAL_INTEGRATE_INTEGRATETIMES_H_ */
