#ifndef INCLUDE_SBMLSIM_INTERNAL_STEPPER_RUNGEKUTTAFEHLBERG78STEPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_STEPPER_RUNGEKUTTAFEHLBERG78STEPPER_H_

#include <boost/numeric/odeint.hpp>
#include "sbmlsim/internal/system/SBMLSystem.h"

using namespace boost::numeric;

template<
    class State,
    class Value = double,
    class Deriv = State,
    class Time = Value,
    class Algebra = typename odeint::algebra_dispatcher<State>::algebra_type,
    class Operations = typename odeint::operations_dispatcher<State>::operations_type,
    class Resizer = odeint::initially_resizer
>
class RungeKuttaFehlberg78Stepper
    : public odeint::runge_kutta_fehlberg78<State, Value, Deriv, Time, Algebra, Operations, Resizer> {
 public:
  typedef odeint::runge_kutta_fehlberg78<State, Value, Deriv, Time, Algebra, Operations, Resizer> stepper_base_type;
  typedef typename stepper_base_type::time_type time_type;
  typedef typename stepper_base_type::algebra_type algebra_type;
 public:
  RungeKuttaFehlberg78Stepper(const algebra_type &algebra = algebra_type()) : stepper_base_type(algebra) {};

  template<class Err>
  void do_step(SBMLSystem system, const SBMLSystem::state &in, const SBMLSystem::state &dxdt, time_type t,
               SBMLSystem::state &out, time_type dt, Err &xerr) {
    this->do_step_impl(system, in, dxdt, t, out, dt, xerr);
  };

  template<class Err>
  void do_step_impl(SBMLSystem system, const SBMLSystem::state &in, const SBMLSystem::state &dxdt,
                    time_type t, SBMLSystem::state &out, time_type dt, Err &xerr) {
    // SBMLSystem::handleEvent
    SBMLSystem::state st = in;
    system.handleEvent(st, t);

    // SBMLSystem::handleReaction
    stepper_base_type::do_step_impl(system, st, dxdt, t, out, dt, xerr);
  }
};

namespace boost { namespace numeric { namespace odeint {

template<class State, class Value, class Deriv, class Time, class Algebra, class Operations, class Resize>
struct get_controller<RungeKuttaFehlberg78Stepper<State, Value, Deriv, Time, Algebra, Operations, Resize> > {
  typedef RungeKuttaFehlberg78Stepper<State ,Value ,Deriv ,Time ,Algebra ,Operations, Resize> stepper_type;
  typedef controlled_runge_kutta<stepper_type> type;
};

} } } /* namespace boost::numeric::odeint */

#endif /* INCLUDE_SBMLSIM_INTERNAL_STEPPER_RUNGEKUTTAFEHLBERG78STEPPER_H_ */
