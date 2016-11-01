#include <memory>
#include <vector>
#include <map>
#include <stack>
#include <cassert>

#include "sbmlsim/SBMLSim.h"
#include "sbmlsim/RunConfiguration.h"
#include "sbmlsim/internal/DevUtil.h"

using namespace std;

void SBMLSim::simulate(const string &filepath, const RunConfiguration &conf) {
  SBMLReader reader;
  SBMLDocument *document = reader.readSBMLFromFile(filepath);
  simulate(document, conf);
  delete document;
}

void SBMLSim::simulate(const SBMLDocument *document, const RunConfiguration &conf) {
  const Model *model = document->getModel();
  const Model *clonedModel = model->clone();
  simulate(clonedModel, conf);
  delete clonedModel;
}

double evaluateASTNode(const ASTNode *node,
    const map<string, double> &speciesValues,
    const map<string, double> &parameterValues) {
  double left, right;
  if (node->getLeftChild() != NULL) {
    left = evaluateASTNode(node->getLeftChild(), speciesValues, parameterValues);
  }
  if (node->getRightChild() != NULL) {
    right = evaluateASTNode(node->getRightChild(), speciesValues, parameterValues);
  }

  switch (node->getType()) {
    case AST_NAME:
      if (speciesValues.find(string(node->getName())) != speciesValues.end()) {
        return speciesValues.at(node->getName());
      }
      if (parameterValues.find(string(node->getName())) != parameterValues.end()) {
        return parameterValues.at(node->getName());
      }
    case AST_PLUS:
      return left + right;
    case AST_MINUS:
      return left - right;
    case AST_TIMES:
      return left * right;
    case AST_DIVIDE:
      return left / right;
    case AST_REAL:
      return node->getReal();
    default:
      assert(false);
      return 0;
  }
}

void SBMLSim::simulate(const Model *model, const RunConfiguration &conf) {
  DevUtil::dumpSBMLDocument(model->getSBMLDocument());

  unsigned int numSpecies = model->getNumSpecies();
  unsigned int numReactions = model->getNumReactions();
  unsigned int numGlobalParameters = model->getNumParameters();

  // initialize species value and index
  map<string, double> speciesValues;
  for (unsigned int i = 0; i < numSpecies; i++) {
    const Species *species = model->getSpecies(i);
    const string speciesId = species->getId();
    double initialAmount = species->getInitialAmount();
    speciesValues[speciesId] = initialAmount;
  }

  // initialize parameter value (global)
  map<string, double> parameterValues;
  for (unsigned int i = 0; i < numGlobalParameters; i++) {
    const Parameter *parameter = model->getParameter(i);
    const string parameterId = parameter->getId();
    double parameterValue = parameter->getValue();
    parameterValues[parameterId] = parameterValue;
  }

  // print reactions
  for (unsigned int i = 0; i < numReactions; i++) {
    const Reaction *reaction = model->getReaction(i);
    const KineticLaw *kineticLaw = reaction->getKineticLaw();
    const ASTNode *node = kineticLaw->getMath();
    DevUtil::dumpASTNode(node);
  }

  // print header
  cout << "t";
  for (unsigned int i = 0; i < numSpecies; i++) {
    const Species *species = model->getSpecies(i);
    const string speciesId = species->getId();
    cout << " " << speciesId;
  }
  cout << endl;

  // simulate (euler)
  double maxt = conf.getDuration();
  double dt = conf.getDt();
  for (double t = 0; t <= maxt; t += dt) {
    // print current
    cout << t;
    for (unsigned int i = 0; i < numSpecies; i++) {
      const Species *species = model->getSpecies(i);
      const string speciesId = species->getId();
      cout << " " << speciesValues[speciesId];
    }
    cout << endl;

    // initialize species diffs
    map<string, double> speciesDiffs;
    for (unsigned int i = 0; i < numSpecies; i++) {
      const Species *species = model->getSpecies(i);
      const string speciesId = species->getId();
      speciesDiffs[speciesId] = 0.0;
    }

    for (unsigned int i = 0; i < numReactions; i++) {
      const Reaction *reaction = model->getReaction(i);
      const KineticLaw *kineticLaw = reaction->getKineticLaw();
      const ASTNode *math = kineticLaw->getMath();

      double val = evaluateASTNode(math, speciesValues, parameterValues);

      unsigned int numReactants = reaction->getNumReactants();
      for (unsigned int i = 0; i < numReactants; i++) {
        const SpeciesReference *reactant = reaction->getReactant(i);
        const string reactantId = reactant->getSpecies();
        speciesDiffs[reactantId] -= val * dt;
      }
      unsigned int numProducts = reaction->getNumProducts();
      for (unsigned int i = 0; i < numProducts; i++) {
        const SpeciesReference *product = reaction->getProduct(i);
        const string productId = product->getSpecies();
        speciesDiffs[productId] += val * dt;
      }
    }

    // update species value
    for (unsigned int i = 0; i < numSpecies; i++) {
      const Species *species = model->getSpecies(i);
      const string speciesId = species->getId();
      speciesValues[speciesId] += speciesDiffs[speciesId];
    }
  }
}
