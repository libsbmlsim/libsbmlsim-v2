#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_MODELWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_MODELWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>
#include <vector>
#include "sbmlsim/internal/wrapper/SpeciesWrapper.h"
#include "sbmlsim/internal/wrapper/ParameterWrapper.h"
#include "sbmlsim/internal/wrapper/CompartmentWrapper.h"
#include "sbmlsim/internal/wrapper/ReactionWrapper.h"
#include "sbmlsim/internal/wrapper/FunctionDefinitionWrapper.h"
#include "sbmlsim/internal/wrapper/EventWrapper.h"
#include "sbmlsim/internal/wrapper/InitialAssignmentWrapper.h"
#include "sbmlsim/internal/wrapper/AssignmentRuleWrapper.h"

class ModelWrapper {
 public:
  explicit ModelWrapper(const Model *model);
  ModelWrapper(const ModelWrapper &model);
  ~ModelWrapper();
  const std::vector<SpeciesWrapper> &getSpecieses() const;
  const std::vector<ParameterWrapper> &getParameters() const;
  const std::vector<CompartmentWrapper> &getCompartments() const;
  const std::vector<ReactionWrapper> &getReactions() const;
  const std::vector<FunctionDefinitionWrapper> &getFunctionDefinitions() const;
  std::vector<EventWrapper *> &getEvents();
  std::vector<InitialAssignmentWrapper *> &getInitialAssignments();
  std::vector<AssignmentRuleWrapper *> &getAssignmentRules();
 private:
  std::vector<SpeciesWrapper> specieses;
  std::vector<ParameterWrapper> parameters;
  std::vector<CompartmentWrapper> compartments;
  std::vector<ReactionWrapper> reactions;
  std::vector<FunctionDefinitionWrapper> functionDefinitions;
  std::vector<EventWrapper *> events;
  std::vector<InitialAssignmentWrapper *> initialAssignments;
  std::vector<AssignmentRuleWrapper *> assignmentRules;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_MODELWRAPPER_H_ */
