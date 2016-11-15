#ifndef INCLUDE_SBMLSIM_SYSTEM_SBMLSYSTEMJACOBI_H_
#define INCLUDE_SBMLSIM_SYSTEM_SBMLSYSTEMJACOBI_H_

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric;

class SBMLSystemJacobi {
 public:
  using state = ublas::vector<double>;
  using matrix = ublas::matrix<double>;
 public:
  void operator()(const state &x, matrix &J, const double &t, state &dfdt);
};

#endif /* INCLUDE_SBMLSIM_SYSTEM_SBMLSYSTEMJACOBI_H_ */
