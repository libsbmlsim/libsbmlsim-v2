#include <sbmlsim/SBMLSim.h>
#include <algorithm>
#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using namespace boost::program_options;

vector<OutputField> createOutputField(string variablesStr, string amountStr, string concentrationStr);

void usage(string myname, options_description opt) {
  cout << "Usage: " << myname << " [options] sbml.xml" << endl;
  cout << opt << endl;
  exit(1);
}

int main(int argc, const char *argv[]) {
  bool isHelp;
  double duration;
  uint32_t steps;
  double delta;
  string variables;
  string amount;
  string concentration;

  options_description optionsGeneral("General options");
  options_description optionsTime("Time");
  options_description optionsVariable("Variable");
  options_description optionsTolerance("Tolerance");
  options_description optionsMethod("Method");
  options_description optionsOutput("Output");

  // clang-format off

  optionsGeneral.add_options()
    ("help,h", "display this help and exit")
    ;

  optionsTime.add_options()
    ("time,t", value<double>(&duration)->default_value(10.0), "specify simulation time")
    ("steps,s", value<uint32_t>(&steps)->default_value(10), "specify steps")
    ("delta,d", value<double>(&delta)->default_value(1.0 / 1024.0, "1.0/1024.0"), "specify delta")
    ;

  optionsVariable.add_options()
    ("variables,v", value<string>(&variables)->default_value(""), "specify variables")
    ("amount,a", value<string>(&amount)->default_value(""), "specify species to output with amount")
    ("concentration,c", value<string>(&concentration)->default_value(""), "specify species to output with concentration")
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

  // clang-format on

  // merge all options
  optionsGeneral.add(optionsTime).add(optionsVariable).add(optionsTolerance).add(optionsMethod).add(optionsOutput);
  variables_map vm;
  string sbml;

  try {
    auto const parse_result = parse_command_line(argc, argv, optionsGeneral);
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

  auto stepInterval = delta * duration / steps;
  auto observeInterval = duration / steps;

  vector<OutputField> outputFields{createOutputField(variables, amount, concentration)};
  RunConfiguration conf(duration, stepInterval, observeInterval, outputFields);

  SBMLSim::simulate(sbml, conf);

  return 0;
}

vector<string> split(const string &s, char delim) {
  vector<string> elems;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    if (!item.empty()) {
      elems.push_back(item);
    }
  }
  return elems;
}

string trim(string &s) {
  auto first = s.find_first_not_of(' ');
  auto last = s.find_last_not_of(' ');
  return s.substr(first, (last - first + 1));
}

vector<OutputField> createOutputField(string variablesStr, string amountStr, string concentrationStr) {
  vector<OutputField> ret;

  vector<string> variables = split(variablesStr, ',');
  for (int i = 0; i < variables.size(); i++) {
    variables[i] = trim(variables[i]);
  }
  vector<string> amount = split(amountStr, ',');
  for (int i = 0; i < amount.size(); i++) {
    amount[i] = trim(amount[i]);
  }
  vector<string> concentration = split(concentrationStr, ',');
  for (int i = 0; i < concentration.size(); i++) {
    concentration[i] = trim(concentration[i]);
  }

  for (int i = 0; i < variables.size(); i++) {
    if (find(amount.begin(), amount.end(), variables[i]) != amount.end()) {
      ret.push_back(OutputField(variables[i], OutputType::AMOUNT));
    } else if (find(concentration.begin(), concentration.end(), variables[i]) != concentration.end()) {
      ret.push_back(OutputField(variables[i], OutputType::CONCENTRATION));
    } else {
      ret.push_back(OutputField(variables[i], OutputType::ASIS));
    }
  }

  return ret;
}
