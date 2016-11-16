#include "sbmlsim/SBMLSim.h"
#include "sbmlsim/system/SBMLSystem.h"
#include "sbmlsim/system/SBMLSystemJacobi.h"
#include "sbmlsim/internal/DevUtil.h"
#include <iostream>
#include <boost/numeric/odeint.hpp>

using namespace boost::numeric;
using state = SBMLSystem::state;

void SBMLSim::simulate(const std::string &filepath, const RunConfiguration &conf) {
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

void SBMLSim::simulate(const Model *model, const RunConfiguration &conf) {
  DevUtil::dumpSBMLDocument(model->getSBMLDocument());

  simulateRungeKutta4(model, conf);
  //simulateRosenbrock4(model, conf);
}

state createInitialState(const Model *model) {
  auto numSpecies = model->getNumSpecies();
  state initialState(numSpecies);
  for (auto i = 0; i < numSpecies; i++) {
    auto *species = model->getSpecies(i);
    auto value = species->getInitialAmount();
    initialState[i] = value;
  }
  return initialState;
}

void SBMLSim::simulateRungeKutta4(const Model *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  odeint::runge_kutta4<state> stepper;
  auto initialState = createInitialState(model);

  // print header
  std::cout << "t";
  for (auto i = 0; i < model->getNumSpecies(); i++) {
    auto *species = model->getSpecies(i);
    std::cout << " " << species->getId();
  }
  std::cout << std::endl;

  // integrate
  integrate_const(
      stepper,
      system,
      initialState,
      conf.getStart(),
      conf.getDuration(),
      conf.getStepInterval(),
      [&](const state& x, const double t) {
        std::cout << t;
        for (int i = 0; i < x.size(); i++) {
          std::cout << " " << x[i];
        }
        std::cout << std::endl;
      }
  );
}

void SBMLSim::simulateRosenbrock4(const Model *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  SBMLSystemJacobi systemJacobi;
  auto initialState = createInitialState(model);
  auto stepper = odeint::make_dense_output(1.0e-6, 1.0e-6, odeint::rosenbrock4<double>());
  auto implicitSystem = std::make_pair(system, systemJacobi);

  // print header
  std::cout << "t";
  for (auto i = 0; i < model->getNumSpecies(); i++) {
    auto *species = model->getSpecies(i);
    std::cout << " " << species->getId();
  }
  std::cout << std::endl;

  // integrate
  integrate_const(
      stepper,
      implicitSystem,
      initialState,
      conf.getStart(),
      conf.getDuration(),
      conf.getStepInterval(),
      [&](const state& x, const double t) {
        std::cout << t;
        for (int i = 0; i < x.size(); i++) {
          std::cout << " " << x[i];
        }
        std::cout << std::endl;
      }
  );
}
