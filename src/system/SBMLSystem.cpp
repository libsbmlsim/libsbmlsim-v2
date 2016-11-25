#include "sbmlsim/internal/system/SBMLSystem.h"
#include <algorithm>
#include "sbmlsim/internal/util/MathUtil.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"
#include "sbmlsim/internal/util/RuntimeExceptionUtil.h"

SBMLSystem::SBMLSystem(const ModelWrapper *model) : model(const_cast<ModelWrapper *>(model)) {
  prepareInitialState();
}

SBMLSystem::SBMLSystem(const SBMLSystem &system)
    : model(system.model), initialState(system.initialState), stateIndexMap(system.stateIndexMap) {
  // nothing to do
}

SBMLSystem::~SBMLSystem() {
  this->initialState.clear();
  this->stateIndexMap.clear();
}

void SBMLSystem::operator()(const state &x, state &dxdt, double t) {
  handleReaction(x, dxdt, t);
}

void SBMLSystem::handleReaction(const state& x, state& dxdt, double t) {
  // initialize
  for (auto i = 0; i < dxdt.size(); i++) {
    dxdt[i] = 0.0;
  }

  auto reactions = model->getReactions();
  for (auto i = 0; i < reactions.size(); i++) {
    auto reaction = reactions[i];
    auto node = reaction.getMath();

    auto clonedNode = ASTNodeUtil::reduceToBinary(node);
    auto value = evaluateASTNode(clonedNode, x);
    delete clonedNode;

    // reactants
    for (auto reactant : reaction.getReactants()) {
      auto index = getStateIndexForVariable(reactant.getSpeciesId());
      double stoichiometry;
      if (reactant.hasStoichiometryMath()) {
        stoichiometry = evaluateASTNode(reactant.getStoichiometryMath(), x);
      } else {
        stoichiometry = reactant.getStoichiometry();
      }
      dxdt[index] -= value * stoichiometry;
    }

    // products
    for (auto product : reaction.getProducts()) {
      auto index = getStateIndexForVariable(product.getSpeciesId());
      double stoichiometry;
      if (product.hasStoichiometryMath()) {
        stoichiometry = evaluateASTNode(product.getStoichiometryMath(), x);
      } else {
        stoichiometry = product.getStoichiometry();
      }
      dxdt[index] += value * stoichiometry;
    }
  }

  // rate rule
  for (auto rateRule : model->getRateRules()) {
    auto index = getStateIndexForVariable(rateRule->getVariable());
    auto value = evaluateASTNode(rateRule->getMath(), x);
    dxdt[index] = value;
  }

  // boundaryCondition and constant
  auto &specieses = model->getSpecieses();
  for (auto i = 0; i < specieses.size(); i++) {
    if (specieses[i].hasBoundaryCondition() || specieses[i].isConstant()) {
      auto index = getStateIndexForVariable(specieses[i].getId());
      dxdt[index] = 0.0;
    }
  }
}

void SBMLSystem::handleEvent(state &x, double t) {
  for (auto event : model->getEvents()) {
    bool fire = evaluateTriggerNode(event->getTrigger(), x);
    if (fire && event->getTriggerState() == false) {
      for (auto eventAssignment : event->getEventAssignments()) {
        auto variable = eventAssignment.getVariable();
        auto index = getStateIndexForVariable(variable);
        double value = evaluateASTNode(eventAssignment.getMath(), x);
        x[index] = value;
        event->setTriggerState(true);
      }
    } else if (!fire) {
      event->setTriggerState(false);
    }
  }
}

void SBMLSystem::handleInitialAssignment(state &x, double t) {
  if (t > 0) {
    return;
  }

  for (auto initialAssignment : model->getInitialAssignments()) {
    auto symbol = initialAssignment->getSymbol();
    auto value = evaluateASTNode(initialAssignment->getMath(), x);

    // species
    auto specieses = model->getSpecieses();
    for (auto i = 0; i < specieses.size(); i++) {
      if (symbol == specieses[i].getId()) {
        auto speciesIndex = getStateIndexForVariable(specieses[i].getId());
        if (specieses[i].shouldMultiplyByCompartmentSizeOnAssignment()) {
          auto compartments = model->getCompartments();
          for (auto j = 0; j < compartments.size(); j++) {
            if (specieses[i].getCompartmentId() == compartments[j].getId()) {
              auto compartmentIndex = getStateIndexForVariable(compartments[j].getId());
              x[speciesIndex] = value * x[compartmentIndex];
            }
          }
        } else {
          x[speciesIndex] = value;
        }
      }
    }

    // compartment
    auto compartments = model->getCompartments();
    for (auto i = 0; i < compartments.size(); i++) {
      if (symbol == compartments[i].getId()) {
        auto index = getStateIndexForVariable(compartments[i].getId());
        x[index] = value;
      }
    }

    // global parameter
    auto parameters = model->getParameters();
    for (auto i = 0; i < parameters.size(); i++) {
      if (symbol == parameters[i]->getId()) {
        auto index = getStateIndexForVariable(parameters[i]->getId());
        x[index] = value;
      }
    }
  }
}

void SBMLSystem::handleAlgebraicRule(state &x, double t) {
  // TODO
}

void SBMLSystem::handleAssignmentRule(state &x, double t) {
  auto &assignmentRules = this->model->getAssignmentRules();
  for (auto i = 0; i < assignmentRules.size(); i++) {
    auto &variable = assignmentRules[i]->getVariable();
    auto value = evaluateASTNode(assignmentRules[i]->getMath(), x);

    bool continueImmediately = false;

    // species
    auto specieses = model->getSpecieses();
    for (auto i = 0; i < specieses.size(); i++) {
      if (variable == specieses[i].getId()) {
        auto speciesIndex = getStateIndexForVariable(specieses[i].getId());
        if (specieses[i].shouldMultiplyByCompartmentSizeOnAssignment()) {
          auto compartments = model->getCompartments();
          for (auto j = 0; j < compartments.size(); j++) {
            if (specieses[i].getCompartmentId() == compartments[j].getId()) {
              auto compartmentIndex = getStateIndexForVariable(compartments[j].getId());
              x[speciesIndex] = value * x[compartmentIndex];
            }
          }
        } else {
          x[speciesIndex] = value;
        }
        continueImmediately = true;
        break;
      }
    }
    if (continueImmediately) {
      continue;
    }

    // compartment
    auto compartments = model->getCompartments();
    for (auto i = 0; i < compartments.size(); i++) {
      if (variable == compartments[i].getId()) {
        auto index = getStateIndexForVariable(compartments[i].getId());
        x[index] = value;
        continueImmediately = true;
        break;
      }
    }
    if (continueImmediately) {
      continue;
    }

    // global parameter
    auto parameters = model->getParameters();
    for (auto i = 0; i < parameters.size(); i++) {
      if (variable == parameters[i]->getId()) {
        auto index = getStateIndexForVariable(parameters[i]->getId());
        x[index] = value;
        break;
      }
    }
  }
}

void SBMLSystem::handleRateRule(state &x, double t) {
  // TODO
}

SBMLSystem::state SBMLSystem::getInitialState() {
  return this->initialState;
}

unsigned int SBMLSystem::getStateIndexForVariable(const std::string &variableId) {
  return this->stateIndexMap[variableId];
}

std::vector<ObserveTarget> SBMLSystem::createOutputTargetsFromOutputFields(
    const std::vector<OutputField> &outputFields) {
  std::vector<ObserveTarget> ret;

  for (auto outputField : outputFields) {
    auto id = outputField.getId();
    auto stateIndex = getStateIndexForVariable(id);
    ret.push_back(ObserveTarget(id, stateIndex));
  }

  return ret;
}

double SBMLSystem::evaluateASTNode(const ASTNode *node, const state& x) {
  double left, right;

  ASTNodeType_t type = node->getType();
  switch (type) {
    case AST_NAME:
      return evaluateNameNode(node, x);
    case AST_PLUS:
      left = evaluateASTNode(node->getLeftChild(), x);
      right = evaluateASTNode(node->getRightChild(), x);
      return left + right;
    case AST_MINUS:
      left = evaluateASTNode(node->getLeftChild(), x);
      right = evaluateASTNode(node->getRightChild(), x);
      return left - right;
    case AST_TIMES:
      left = evaluateASTNode(node->getLeftChild(), x);
      right = evaluateASTNode(node->getRightChild(), x);
      return left * right;
    case AST_DIVIDE:
      left = evaluateASTNode(node->getLeftChild(), x);
      right = evaluateASTNode(node->getRightChild(), x);
      return left / right;
    case AST_POWER:
    case AST_FUNCTION_POWER:
      left = evaluateASTNode(node->getLeftChild(), x);
      right = evaluateASTNode(node->getRightChild(), x);
      return MathUtil::pow(left, right);
    case AST_REAL:
      return node->getReal();
    case AST_INTEGER:
      return node->getInteger();
    case AST_RATIONAL:
    case AST_REAL_E:
      return node->getValue();
    case AST_FUNCTION_FACTORIAL:
      return evaluateFactorialNode(node, x);
    case AST_FUNCTION_CEILING:
      return MathUtil::ceil(evaluateASTNode(node->getLeftChild(), x));
    case AST_FUNCTION_PIECEWISE:
      return evaluatePiecewiseNode(node, x);
    default:
      std::cout << "type = " << type << std::endl;
      break;
  }
  return 0;
}

double SBMLSystem::evaluateNameNode(const ASTNode *node, const state &x) {
  auto name = node->getName();

  // species
  auto specieses = model->getSpecieses();
  for (auto i = 0; i < specieses.size(); i++) {
    if (name == specieses[i].getId()) {
      if (specieses[i].shouldDivideByCompartmentSizeOnEvaluation()) {
        auto compartments = model->getCompartments();
        for (auto j = 0; j < compartments.size(); j++) {
          if (specieses[i].getCompartmentId() == compartments[j].getId()) {
            auto speciesIndex = getStateIndexForVariable(specieses[i].getId());
            auto compartmentIndex = getStateIndexForVariable(compartments[j].getId());
            return x[speciesIndex] / x[compartmentIndex];
          }
        }
      } else {
        auto speciesIndex = getStateIndexForVariable(specieses[i].getId());
        return x[speciesIndex];
      }
    }
  }

  // compartment
  auto compartments = model->getCompartments();
  for (auto i = 0; i < compartments.size(); i++) {
    if (name == compartments[i].getId()) {
      auto index = getStateIndexForVariable(compartments[i].getId());
      return x[index];
    }
  }

  // global parameter
  auto parameters = model->getParameters();
  for (auto i = 0; i < parameters.size(); i++) {
    if (name == parameters[i]->getId()) {
      auto index = getStateIndexForVariable(parameters[i]->getId());
      return x[index];
    }
  }

  // not reachable
  RuntimeExceptionUtil::throwUnknownNodeNameException(name);
  return 0.0;
}

double SBMLSystem::evaluateFactorialNode(const ASTNode *node, const state &x) {
  const ASTNode *left = node->getLeftChild();
  long long leftValue;
  ASTNodeType_t leftType = left->getType();
  switch (leftType) {
    case AST_FUNCTION_CEILING:
      leftValue = MathUtil::ceil(evaluateASTNode(left->getLeftChild(), x));
      return MathUtil::factorial(leftValue);
    default:
      break;
  }

  // not reachable
  std::cout << "left node type = " << leftType << std::endl;
  RuntimeExceptionUtil::throwUnknownNodeTypeException(leftType);
  return 0.0;
}

double SBMLSystem::evaluatePiecewiseNode(const ASTNode *node, const state &x) {
  // piece nodes
  for (auto i = 0; i < node->getNumChildren() - 1; i += 2) {
    auto conditionalNode = node->getChild(i + 1);
    if (evaluatePiecewiseConditionalNode(conditionalNode, x)) {
      return evaluateASTNode(node->getChild(i), x);
    }
  }

  // otherwise node
  auto otherwiseNode = node->getRightChild();
  return evaluateASTNode(otherwiseNode, x);
}

bool SBMLSystem::evaluatePiecewiseConditionalNode(const ASTNode *node, const state &x) {
  auto type = node->getType();
  switch (type) {
    case AST_CONSTANT_FALSE:
      return false;
    default:
      break;
  }

  // not reachable
  std::cout << "conditional node type = " << type << std::endl;
  RuntimeExceptionUtil::throwUnknownNodeTypeException(type);
  return false;
}

bool SBMLSystem::evaluateTriggerNode(const ASTNode *trigger, const state &x) {
  double left, right;

  auto type = trigger->getType();
  switch (type) {
    case AST_RELATIONAL_LT:
      left = evaluateASTNode(trigger->getLeftChild(), x);
      right = evaluateASTNode(trigger->getRightChild(), x);
      return left < right;
    default:
      break;
  }

  // not reachable
  std::cout << "trigger root node type = " << type << std::endl;
  RuntimeExceptionUtil::throwUnknownNodeTypeException(type);
  return false;
}

void SBMLSystem::prepareInitialState() {
  auto &specieses = this->model->getSpecieses();
  auto numSpecies = specieses.size();

  auto &parameters = this->model->getParameters();
  auto numGlobalParameters = parameters.size();

  auto &compartments = this->model->getCompartments();
  auto numCompartments = compartments.size();

  auto numVariables = numSpecies + numGlobalParameters + numCompartments;
  state is(numVariables);

  auto curIndex = 0;
  for (auto i = 0; i < numSpecies; i++) {
    this->stateIndexMap[specieses[i].getId()] = curIndex;
    is[curIndex++] = specieses[i].getInitialAmountValue();
  }
  for (auto i = 0; i < parameters.size(); i++) {
    this->stateIndexMap[parameters[i]->getId()] = curIndex;
    is[curIndex++] = parameters[i]->getValue();
  }
  for (auto i = 0; i < numCompartments; i++) {
    this->stateIndexMap[compartments[i].getId()] = curIndex;
    is[curIndex++] = compartments[i].getValue();
  }

  this->initialState = is;
}
