#ifndef INCLUDE_SBMLSIM_SYSTEM_SBMLSYSTEM_H_
#define INCLUDE_SBMLSIM_SYSTEM_SBMLSYSTEM_H_

#include <sbml/SBMLTypes.h>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric;

class SBMLSystem {
 public:
  using state = ublas::vector<double>;
 public:
  explicit SBMLSystem(const Model *model);
  void operator()(const state &x, state &dxdt, double t);
 private:
  const Model *model;
  double evaluateASTNode(const ASTNode *node, int reactionIndex, const state &x);
  int getIndexForSpecies(std::string speciesId);
};

#endif /* INCLUDE_SBMLSIM_SYSTEM_SBMLSYSTEM_H_ */
