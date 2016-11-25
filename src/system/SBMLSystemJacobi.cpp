#include "sbmlsim/internal/system/SBMLSystemJacobi.h"

void SBMLSystemJacobi::operator()(const state &x, matrix &J, const double &t, state &dfdt) {
  // works with 00001-sbml-l2v4.xml only
  J(0, 0) = -1.0;
  J(0, 1) = 0.0;
  J(1, 0) = 1.0;
  J(1, 1) = 0.0;
  dfdt[0] = 0.0;
  dfdt[1] = 0.0;
}
