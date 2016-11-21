#include "sbmlsim/SBMLSim.h"

#include <iostream>
#include <boost/numeric/odeint.hpp>
#include "sbmlsim/internal/system/SBMLSystem.h"
#include "sbmlsim/internal/system/SBMLSystemJacobi.h"
#include "sbmlsim/internal/integrate/IntegrateConst.h"
#include "sbmlsim/internal/observer/StdoutCsvObserver.h"
#include "sbmlsim/internal/util/DevUtil.h"

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

  ModelWrapper *modelWrapper = new ModelWrapper(model);

  // simulateRungeKutta4(modelWrapper, conf);
  // simulateRungeKuttaDopri5(modelWrapper, conf);
  simulateRungeKuttaFehlberg78(modelWrapper, conf);
  // simulateRosenbrock4(modelWrapper, conf);

  delete modelWrapper;
}

state createInitialState(const ModelWrapper *model) {
  const std::vector<SpeciesWrapper> &specieses = model->getSpecieses();

  auto numSpecies = specieses.size();
  state initialState(numSpecies);
  for (auto i = 0; i < numSpecies; i++) {
    auto species = specieses[i];
    auto value = species.getInitialValue();
    initialState[i] = value;
  }

  return initialState;
}

void SBMLSim::simulateRungeKutta4(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  odeint::runge_kutta4<state> stepper;
  auto initialState = createInitialState(model);
  StdoutCsvObserver observer;

  // print header
  std::vector<SpeciesWrapper> specieses = model->getSpecieses();
  std::cout << "t";
  for (auto i = 0; i < specieses.size(); i++) {
    auto species = specieses[i];
    std::cout << "," << species.getId();
  }
  std::cout << std::endl;

  // integrate
  integrate_const(stepper, system, initialState, conf.getStart(), conf.getDuration(), conf.getStepInterval(),
                  std::ref(observer));
}

void SBMLSim::simulateRungeKuttaDopri5(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  auto stepper = odeint::make_controlled<odeint::runge_kutta_dopri5<state> >(
      conf.getAbsoluteTolerance(), conf.getRelativeTolerance());
  auto initialState = createInitialState(model);
  StdoutCsvObserver observer;

  // print header
  std::vector<SpeciesWrapper> specieses = model->getSpecieses();
  std::cout << "t";
  for (auto i = 0; i < specieses.size(); i++) {
    auto species = specieses[i];
    std::cout << "," << species.getId();
  }
  std::cout << std::endl;

  // integrate
  integrate_const(stepper, system, initialState, conf.getStart(), conf.getDuration(), conf.getStepInterval(),
                  std::ref(observer));
}

void SBMLSim::simulateRungeKuttaFehlberg78(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  auto stepper = odeint::make_controlled<odeint::runge_kutta_fehlberg78<state> >(
      conf.getAbsoluteTolerance(), conf.getRelativeTolerance());
  auto initialState = createInitialState(model);
  StdoutCsvObserver observer;

  // print header
  std::vector<SpeciesWrapper> specieses = model->getSpecieses();
  std::cout << "t";
  for (auto i = 0; i < specieses.size(); i++) {
    auto species = specieses[i];
    std::cout << "," << species.getId();
  }
  std::cout << std::endl;

  // integrate
  sbmlsim::integrate_const(
      stepper, system, initialState, conf.getStart(), conf.getDuration(), conf.getStepInterval(), std::ref(observer));
}

void SBMLSim::simulateRosenbrock4(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  SBMLSystemJacobi systemJacobi;
  auto initialState = createInitialState(model);
  auto stepper = odeint::make_dense_output(conf.getAbsoluteTolerance(), conf.getRelativeTolerance(),
                                           odeint::rosenbrock4<double>());
  auto implicitSystem = std::make_pair(system, systemJacobi);
  StdoutCsvObserver observer;

  // print header
  std::vector<SpeciesWrapper> specieses = model->getSpecieses();
  std::cout << "t";
  for (auto i = 0; i < specieses.size(); i++) {
    auto species = specieses[i];
    std::cout << "," << species.getId();
  }
  std::cout << std::endl;

  // integrate
  integrate_const(stepper, implicitSystem, initialState, conf.getStart(), conf.getDuration(), conf.getStepInterval(),
                  std::ref(observer));
}
