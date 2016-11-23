#ifndef INCLUDE_SBMLSIM_INTERNAL_SYSTEM_SBMLSYSTEM_H_
#define INCLUDE_SBMLSIM_INTERNAL_SYSTEM_SBMLSYSTEM_H_

#include <sbml/SBMLTypes.h>
#include <string>
#include <boost/numeric/ublas/vector.hpp>
#include "sbmlsim/internal/wrapper/ModelWrapper.h"

using namespace boost::numeric;

class SBMLSystem {
 public:
  using state = ublas::vector<double>;
 public:
  explicit SBMLSystem(const ModelWrapper *model);
  SBMLSystem(const SBMLSystem &system);
  ~SBMLSystem();
  void operator()(const state &x, state &dxdt, double t);
  void handleReaction(const state &x, state &dxdt, double t);
  void handleEvent(state &x, double t);
  void handleInitialAssignment(state &x, double t);
  void handleRule(state &x, double t);
  void handleAlgebraicRule(state &x, double t);
  void handleAssignmentRule(state &x, double t);
  void handleRateRule(state &x, double t);
 private:
  ModelWrapper *model;
  double evaluateASTNode(const ASTNode *node, int reactionIndex, const state &x);
  double evaluateNameNode(const ASTNode *node, int reactionIndex, const state &x);
  double evaluateFunctionNode(const ASTNode *node, int reactionIndex, const state &x);
  double evaluateFactorialNode(const ASTNode *node, int reactionIndex, const state &x);
  bool evaluateTriggerNode(const ASTNode *trigger, const state &x);
  int getIndexForSpecies(const std::string &speciesId);
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_SYSTEM_SBMLSYSTEM_H_ */
