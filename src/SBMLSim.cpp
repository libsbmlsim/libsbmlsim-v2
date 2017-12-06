#include "sbmlsim/SBMLSim.h"

#include <iostream>
#include <boost/numeric/odeint.hpp>
#include "sbmlsim/internal/system/SBMLSystem.h"
#include "sbmlsim/internal/system/SBMLSystemJacobi.h"
#include "sbmlsim/internal/integrate/IntegrateConst.h"
#include "sbmlsim/internal/observer/StdoutCsvObserver.h"
#include "sbmlsim/internal/thirdparty/liblsoda.h"

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
  unsigned int level = document->getLevel();
  unsigned int version = document->getVersion();
  simulate(model, level, version, conf);
}

void SBMLSim::simulate(const Model *model, unsigned int level, unsigned int version, const RunConfiguration &conf) {
  Model *clonedModel = model->clone();
  SBMLDocument *dummyDocument = new SBMLDocument(level, version);
  clonedModel->setSBMLDocument(dummyDocument);
  dummyDocument->setModel(clonedModel);

  ModelWrapper *modelWrapper = new ModelWrapper(clonedModel);

  // simulateRungeKutta4(modelWrapper, conf);
  simulateRungeKuttaDopri5(modelWrapper, conf);
  // simulateRungeKuttaFehlberg78(modelWrapper, conf);
  // simulateRosenbrock4(modelWrapper, conf);
  // simulateLSODA(modelWrapper, conf);

  delete modelWrapper;
  delete dummyDocument;
}

void SBMLSim::simulateRungeKutta4(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  odeint::runge_kutta4<state> stepper;
  auto initialState = system.getInitialState();
  StdoutCsvObserver observer(system.createOutputTargetsFromOutputFields(conf.getOutputFields()));

  // print header
  observer.outputHeader();

  // integrate
  sbmlsim::integrate_const(
      stepper, system, initialState, conf.getStart(), conf.getDuration(), conf.getStepInterval(), std::ref(observer));
}

void SBMLSim::simulateRungeKuttaDopri5(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  auto stepper = odeint::make_controlled<odeint::runge_kutta_dopri5<state> >(
      conf.getAbsoluteTolerance() / 100.0, conf.getRelativeTolerance() / 100.0);
  auto initialState = system.getInitialState();
  StdoutCsvObserver observer(system.createOutputTargetsFromOutputFields(conf.getOutputFields()));

  // print header
  observer.outputHeader();

  // integrate
  sbmlsim::integrate_const(
      stepper, system, initialState, conf.getStart(), conf.getDuration(), conf.getStepInterval(), std::ref(observer));
}

void SBMLSim::simulateRungeKuttaFehlberg78(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  auto stepper = odeint::make_controlled<odeint::runge_kutta_fehlberg78<state> >(
      conf.getAbsoluteTolerance() / 100.0, conf.getRelativeTolerance() / 100.0);
  auto initialState = system.getInitialState();
  StdoutCsvObserver observer(system.createOutputTargetsFromOutputFields(conf.getOutputFields()));

  // print header
  observer.outputHeader();

  // integrate
  sbmlsim::integrate_const(
      stepper, system, initialState, conf.getStart(), conf.getDuration(), conf.getStepInterval(), std::ref(observer));
}

void SBMLSim::simulateRosenbrock4(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  SBMLSystemJacobi systemJacobi;
  auto initialState = system.getInitialState();
  auto stepper = odeint::make_dense_output(conf.getAbsoluteTolerance() / 100.0, conf.getRelativeTolerance() / 100.0,
                                           odeint::rosenbrock4<double>());
  auto implicitSystem = std::make_pair(system, systemJacobi);
  StdoutCsvObserver observer(system.createOutputTargetsFromOutputFields(conf.getOutputFields()));

  // print header
  observer.outputHeader();

  // integrate
  integrate_const(stepper, implicitSystem, initialState, conf.getStart(), conf.getDuration(), conf.getStepInterval(),
                  std::ref(observer));
}

/*****************************
 * TODO implement with LSODA *
 *****************************/

int fex(double t, double *y, double *ydot, void *data) {
  ydot[0] = - 0.1 * y[0];
  ydot[1] = 0.1 * y[0];
  return 0;
}

void SBMLSim::simulateLSODA(const ModelWrapper *model, const RunConfiguration &conf) {
  int neq = 2;
  double rtol[] = {1e-4, 1e-4};
  double atol[] = {1e-4, 1e-4};
  double y[] = {10.0, 0.0};

  lsoda_opt_t opt = {0};
  opt.ixpr = 0;
  opt.rtol = rtol;
  opt.atol = atol;
  opt.itask = 1;

  lsoda_context_t ctx = {0};
  ctx.function = fex;
  ctx.neq = neq;
  ctx.data = NULL;
  ctx.state = 1;

  lsoda_prepare(&ctx, &opt);

  double t = 0.0;
  for (double tout = 0.1; tout <= 4.0; tout += 0.1) {
    lsoda(&ctx, y, &t, tout);
    std::cout << t << " " << tout << " " << y[0] << " " << y[1] << std::endl;

    if (ctx.state <= 0) {
      std::cout << "[ERROR] ctx.stat <= 0" << std::endl;
    }
  }

  lsoda_free(&ctx);
}
