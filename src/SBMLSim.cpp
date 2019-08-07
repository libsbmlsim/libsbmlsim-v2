#include "sbmlsim/SBMLSim.h"

#include <boost/numeric/odeint.hpp>
#include <iostream>
#include <vector>
#include "sbmlsim/internal/integrate/IntegrateTimes.h"
#include "sbmlsim/internal/observer/StdoutCsvObserver.h"
#include "sbmlsim/internal/system/SBMLSystem.h"
#include "sbmlsim/internal/thirdparty/liblsoda.h"

using boost::numeric::odeint::runge_kutta4;
using std::ref;
using std::string;
using std::vector;
// Useless comment to check my git setup
using namespace boost::numeric;
using boost::numeric::odeint::runge_kutta4;
using std::ref;
using std::string;
using std::vector;
using state = SBMLSystem::state;

void SBMLSim::simulate(const string &filepath, const RunConfiguration &conf) {
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
  // simulateLSODA(modelWrapper, conf);

  delete modelWrapper;
  delete dummyDocument;
}

void SBMLSim::simulateRungeKutta4(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  runge_kutta4<state> stepper;
  auto initialState = system.getInitialState();
  StdoutCsvObserver observer(system.createOutputTargetsFromOutputFields(conf.getOutputFields()));

  // prepare TimeIterator
  vector<double> observeTimes;
  auto t_stop = conf.getDuration() + conf.getObserveInterval() / 2;
  for (double t = 0.0; t <= t_stop; t += conf.getObserveInterval()) {
    observeTimes.push_back(t);
  }

  // print header
  observer.outputHeader();

  // integrate
  sbmlsim::integrate_times(stepper, system, initialState, observeTimes.begin(), observeTimes.end(),
                           conf.getStepInterval(), ref(observer));
}

void SBMLSim::simulateRungeKuttaDopri5(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  auto stepper = odeint::make_controlled<odeint::runge_kutta_dopri5<state> >(conf.getAbsoluteTolerance(),
                                                                             conf.getRelativeTolerance());
  auto initialState = system.getInitialState();
  StdoutCsvObserver observer(system.createOutputTargetsFromOutputFields(conf.getOutputFields()));

  // prepare TimeIterator
  vector<double> observeTimes;
  auto t_stop = conf.getDuration() + conf.getObserveInterval() / 2;
  for (double t = 0.0; t <= t_stop; t += conf.getObserveInterval()) {
    observeTimes.push_back(t);
  }

  // print header
  observer.outputHeader();

  // integrate
  sbmlsim::integrate_times(stepper, system, initialState, observeTimes.begin(), observeTimes.end(),
                           conf.getObserveInterval(), ref(observer));
}

void SBMLSim::simulateRungeKuttaFehlberg78(const ModelWrapper *model, const RunConfiguration &conf) {
  SBMLSystem system(model);
  auto stepper = odeint::make_controlled<odeint::runge_kutta_fehlberg78<state> >(conf.getAbsoluteTolerance(),
                                                                                 conf.getRelativeTolerance());
  auto initialState = system.getInitialState();
  StdoutCsvObserver observer(system.createOutputTargetsFromOutputFields(conf.getOutputFields()));

  // prepare TimeIterator
  vector<double> observeTimes;
  auto t_stop = conf.getDuration() + conf.getObserveInterval() / 2;
  for (double t = 0.0; t <= t_stop; t += conf.getObserveInterval()) {
    observeTimes.push_back(t);
  }

  // print header
  observer.outputHeader();

  // integrate
  sbmlsim::integrate_times(stepper, system, initialState, observeTimes.begin(), observeTimes.end(),
                           conf.getObserveInterval(), ref(observer));
}

/*****************************
 * TODO implement with LSODA *
 *****************************/

/*
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
*/
