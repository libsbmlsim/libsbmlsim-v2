#include <sbmlsim/SBMLSim.h>

using std::vector;

int main(int argc, const char *argv[]) {
  double start{0.0};
  double duration{10.0};
  double stepInterval{0.01};
  double observeInterval{0.1};
  vector<OutputField> outputFiels{OutputField("S1", OutputType::AMOUNT), OutputField("S2", OutputType::CONCENTRATION)};

  RunConfiguration conf(start, duration, stepInterval, observeInterval, outputFiels);

  SBMLSim::simulate(argv[1], conf);

  return 0;
}
