#include <sbmlsim/SBMLSim.h>
#include <iostream>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

void usage(string myname, options_description opt) {
  cout << "Usage: " << myname << " [options] sbml.xml" << endl;
  cout << opt << endl;
  exit(0);
}

int main(int argc, const char* argv[]) {
  bool isHelp;
  double duration;
  double stepInterval;

  options_description optionsGeneral("General options");
  options_description optionsTime("Time");
  options_description optionsTolerance("Tolerance");
  options_description optionsMethod("Method");
  options_description optionsOutput("Output");

  optionsGeneral.add_options()
    ("help,h", "display this help and exit")
    ;

  optionsTime.add_options()
    ("time,t", value<double>(&duration)->default_value(10.0, "10.0"), "specify simulation time")
    ("dt,d", value<double>(&stepInterval)->default_value(0.1, "0.1"), "specify delta t")
    ;

  optionsTolerance.add_options()
    ("absolute,A", value<double>()->default_value(1e-09, "1e-09"), "specify absolute tolerance for variable stepsize")
    ("relative,R", value<double>()->default_value(1e-06, "1e-06"), "specify relative tolerance for variable stepsize")
    ;

  int method;
  optionsMethod.add_options()
    ("method,m", value<int>(&method)->default_value(1), "specify numerical integration algorithm")
    ;

  string outputFile;
  optionsOutput.add_options()
    ("outputFile,o", value<string>(&outputFile)->default_value("out.csv"), "specify output filename")
    ;

  // merge all options
  optionsGeneral.add(optionsTime).add(optionsTolerance).add(optionsMethod).add(optionsOutput);
  variables_map vm;
  string sbml;

  try {
    auto const parse_result  = parse_command_line(argc, argv, optionsGeneral);
    store(parse_result, vm);
    notify(vm);
    auto array = collect_unrecognized(parse_result.options, include_positional);
    if (array.size() != 1 || argc == 1 || vm.count("help")) {
      usage(argv[0], optionsGeneral);
    }
    sbml = collect_unrecognized(parse_result.options, include_positional)[0];
  } catch (exception &e) {
    cout << e.what() << endl;
    usage(argv[0], optionsGeneral);
  }

  // cout << "    SBML: [" << sbml << "]" << endl;
  // cout << "duration: [" << duration << "]" << endl;
  // cout << "      dt: [" << stepInterval << "]" << endl;

  RunConfiguration conf(duration, stepInterval);

  SBMLSim::simulate(sbml, conf);

  return 0;
}
