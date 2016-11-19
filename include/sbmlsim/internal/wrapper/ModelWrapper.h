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
 private:
  std::vector<SpeciesWrapper> specieses;
  std::vector<ParameterWrapper> parameters;
  std::vector<CompartmentWrapper> compartments;
  std::vector<ReactionWrapper> reactions;
  std::vector<FunctionDefinitionWrapper> functionDefinitions;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_MODELWRAPPER_H_ */
