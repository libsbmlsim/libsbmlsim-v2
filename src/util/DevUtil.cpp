#include "sbmlsim/internal/DevUtil.h"

#include <cassert>

using namespace std;

void DevUtil::dumpSBMLSpecInfo(const SBMLDocument *document) {
#ifndef NDEBUG
  if (document == NULL) {
    return;
  }

  unsigned int level = document->getLevel();
  unsigned int version = document->getVersion();
  cout << "level = " << level << ", version = " << version << endl;
#endif
}

void DevUtil::dumpSBMLDocument(const SBMLDocument *document) {
#ifndef NDEBUG
  if (document == NULL) {
    return;
  }

  const Model *model = document->getModel();

  // level, version
  unsigned int level = document->getLevel();
  cout << "[document] level = " << level << endl;
  unsigned int version = document->getVersion();
  cout << "[document] version = " << version << endl;

  // species
  unsigned int numSpecies = model->getNumSpecies();
  for (unsigned int i = 0; i < numSpecies; i++) {
    const Species *species = model->getSpecies(i);
    const string speciesId = species->getId();
    cout << "[species] speciesId = " << speciesId << endl;
  }

  // compartment
  unsigned int numCompartments = model->getNumCompartments();
  for (unsigned int i = 0; i < numCompartments; i++) {
    const Compartment *compartment = model->getCompartment(i);
    const string compartmentId = compartment->getId();
    cout << "[compartment] compartmentId = " << compartmentId << endl;
  }

  // parameters (global)
  unsigned int numGlobalParameters = model->getNumParameters();
  for (unsigned int i = 0; i < numGlobalParameters; i++) {
    const Parameter *parameter = model->getParameter(i);
    const string parameterId = parameter->getId();
    double parameterValue = parameter->getValue();
    cout << "[parameter] " << parameterId << " = " << parameterValue << endl;
  }

  // reactions
  // constraints
  // events
  // functiondefinitions
  // initialassignments
#endif
}

void DevUtil::dumpASTNode(const ASTNode *node) {
#ifndef NDEBUG
  const ASTNode *leftNode = node->getLeftChild();
  if (leftNode != NULL) {
    dumpASTNode(leftNode);
  }

  const ASTNode *rightNode = node->getRightChild();
  if (rightNode != NULL) {
    dumpASTNode(rightNode);
  }

  switch (node->getType()) {
    case AST_NAME:
      cout << node->getName() << endl;
      break;
    case AST_PLUS:
    case AST_MINUS:
    case AST_TIMES:
    case AST_DIVIDE:
    case AST_POWER:
      cout << node->getOperatorName() << endl;
      break;
    default:
      assert(false);
      break;
  }
#endif
}
