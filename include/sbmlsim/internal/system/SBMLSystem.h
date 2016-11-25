#ifndef INCLUDE_SBMLSIM_INTERNAL_SYSTEM_SBMLSYSTEM_H_
#define INCLUDE_SBMLSIM_INTERNAL_SYSTEM_SBMLSYSTEM_H_

#include <sbml/SBMLTypes.h>
#include <string>
#include <unordered_map>
#include <boost/numeric/ublas/vector.hpp>
#include "sbmlsim/internal/wrapper/ModelWrapper.h"
#include "sbmlsim/config/OutputField.h"
#include "sbmlsim/internal/observer/ObserveTarget.h"

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
  void handleAlgebraicRule(state &x, double t);
  void handleAssignmentRule(state &x, double t);
  state getInitialState();
  unsigned int getStateIndexForVariable(const std::string &variableId);
  std::vector<ObserveTarget> createOutputTargetsFromOutputFields(const std::vector<OutputField> &outputFields);
 private:
  ModelWrapper *model;
  state initialState;
  std::unordered_map<std::string, unsigned int> stateIndexMap;
  void handleRateRule(const state &x, state &dxdt, double t);
  double evaluateASTNode(const ASTNode *node, const state &x);
  double evaluateNameNode(const ASTNode *node, const state &x);
  double evaluateFunctionNode(const ASTNode *node, const state &x);
  double evaluateFactorialNode(const ASTNode *node, const state &x);
  double evaluatePiecewiseNode(const ASTNode *node, const state &x);
  bool evaluateConditionalNode(const ASTNode *node, const state &x);
  void prepareInitialState();
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_SYSTEM_SBMLSYSTEM_H_ */
