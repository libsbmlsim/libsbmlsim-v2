#include "sbmlsim/internal/wrapper/ModelWrapper.h"

ModelWrapper::ModelWrapper(const Model *model) {
  // species
  for (auto i = 0; i < model->getNumSpecies(); i++) {
    auto species = model->getSpecies(i);
    this->specieses.push_back(SpeciesWrapper(species));
  }

  // global parameters
  for (auto i = 0; i < model->getNumParameters(); i++) {
    auto parameter = model->getParameter(i);
    this->parameters.push_back(ParameterWrapper::asGlobalParameter(parameter));
  }

  // local parameters
  for (auto i = 0; i < model->getNumReactions(); i++) {
    auto reaction = model->getReaction(i);
    auto kineticLaw = reaction->getKineticLaw();
    for (auto j = 0; j < kineticLaw->getNumParameters(); j++) {
      auto parameter = kineticLaw->getParameter(j);
      this->parameters.push_back(ParameterWrapper::asLocalParameter(parameter, reaction));
    }
  }

  // compartments
  for (auto i = 0; i < model->getNumCompartments(); i++) {
    auto compartment = model->getCompartment(i);
    this->compartments.push_back(CompartmentWrapper(compartment));
  }

  // reactions
  for (auto i = 0; i < model->getNumReactions(); i++) {
    auto reaction = model->getReaction(i);
    this->reactions.push_back(ReactionWrapper(reaction));
  }

  // function definitions
  for (auto i = 0; i < model->getNumFunctionDefinitions(); i++) {
    auto functionDefinition = model->getFunctionDefinition(i);
    this->functionDefinitions.push_back(new FunctionDefinitionWrapper(functionDefinition));
  }

  // events
  for (auto i = 0; i < model->getNumEvents(); i++) {
    auto event = model->getEvent(i);
    this->events.push_back(new EventWrapper(event));
  }

  // initial assignments
  for (auto i = 0; i < model->getNumInitialAssignments(); i++) {
    auto initialAssignment = model->getInitialAssignment(i);
    this->initialAssignments.push_back(new InitialAssignmentWrapper(initialAssignment));
  }

  // rules
  for (auto i = 0; i < model->getNumRules(); i++) {
    auto rule = model->getRule(i);
    if (rule->isAssignment()) {
      const AssignmentRule *assignmentRule = static_cast<const AssignmentRule *>(rule);
      this->assignmentRules.push_back(new AssignmentRuleWrapper(assignmentRule));
    } else if (rule->isRate()) {
      const RateRule *rateRule = static_cast<const RateRule *>(rule);
      this->rateRules.push_back(new RateRuleWrapper(rateRule));
    }
  }
}

ModelWrapper::ModelWrapper(const ModelWrapper &model) {
  this->specieses = model.specieses;
  this->parameters = model.parameters;
  this->compartments = model.compartments;
  this->reactions = model.reactions;
  this->functionDefinitions = model.functionDefinitions;
  this->events = model.events;
  this->initialAssignments = model.initialAssignments;
  this->assignmentRules = model.assignmentRules;
}

ModelWrapper::~ModelWrapper() {
  this->specieses.clear();

  for (auto parameter : this->parameters) {
    delete parameter;
  }
  this->parameters.clear();

  this->compartments.clear();
  this->reactions.clear();

  for (auto functionDefinition : this->functionDefinitions) {
    delete functionDefinition;
  }
  this->functionDefinitions.clear();

  for (auto event : this->events) {
    delete event;
  }
  this->events.clear();

  for (auto initialAssignment : this->initialAssignments) {
    delete initialAssignment;
  }
  this->initialAssignments.clear();

  for (auto assignmentRule : this->assignmentRules) {
    delete assignmentRule;
  }
  this->assignmentRules.clear();
}

const std::vector<SpeciesWrapper> &ModelWrapper::getSpecieses() const {
  return this->specieses;
}

std::vector<ParameterWrapper *> &ModelWrapper::getParameters() {
  return this->parameters;
}

const std::vector<CompartmentWrapper> &ModelWrapper::getCompartments() const {
  return this->compartments;
}

const std::vector<ReactionWrapper> &ModelWrapper::getReactions() const {
  return this->reactions;
}

std::vector<FunctionDefinitionWrapper *> &ModelWrapper::getFunctionDefinitions() {
  return this->functionDefinitions;
}

std::vector<EventWrapper *> &ModelWrapper::getEvents() {
  return this->events;
}

std::vector<InitialAssignmentWrapper *> &ModelWrapper::getInitialAssignments() {
  return this->initialAssignments;
}

std::vector<AssignmentRuleWrapper *> &ModelWrapper::getAssignmentRules() {
  return this->assignmentRules;
}

std::vector<RateRuleWrapper *> &ModelWrapper::getRateRules() {
  return this->rateRules;
}
