#include <sbmlsim/SBMLSim.h>
#include <iostream>

using namespace std;

int main(int argc, const char* argv[]) {
  double duration = 10.0;
  double steps = 0.1;

  RunConfiguration conf(duration, steps);

  SBMLSim::simulate(argv[1], conf);

  return 0;
}

