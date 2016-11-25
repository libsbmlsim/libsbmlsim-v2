#include <sbmlsim/SBMLSim.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

void usage(const string &bin);
RunConfiguration readSetting(const string &caseDirPath, const string &modelNumber);
vector<string> split(const string &s, char delim);
string trim(string &s);

int main(int argc, const char* argv[]) {
  if (argc < 3) {
    usage(string(argv[0]));
    return 1;
  }

  string caseDirPath(argv[1]);
  string modelNumber(argv[2]);
  auto modelPath = caseDirPath + "/" + modelNumber + "/" + modelNumber + "-sbml-l2v5.xml";

  auto conf = readSetting(caseDirPath, modelNumber);
  SBMLSim::simulate(modelPath, conf);

  return 0;
}

void usage(const string &bin) {
  cout << "Usage: " << bin << "[caseDirPath] [modelNumber]" << endl;
  cout << "Example: " << bin << "./sbml-test-suite/cases/semantic 00001" << endl;
}

RunConfiguration readSetting(const string &caseDirPath, const string &modelNumber) {
  auto modelDirPath = caseDirPath + "/" + modelNumber;
  auto settingFilename = modelNumber + "-settings.txt";

  double start;
  double duration;
  double steps;
  vector<string> variables;
  double absolute;
  double relative;
  vector<string> amount;
  vector<string> concentration;

  ifstream ifs(modelDirPath + "/" + settingFilename);

  /*
   * [settings sample]
   *
   * start: 0
   * duration: 5
   * steps: 50
   * variables: S1, S2
   * absolute: 1.000000e-007
   * relative: 0.0001
   * amount: S1, S2
   * concentration:
   */

  string line;
  while (getline(ifs, line)) {
    vector<string> colonSeparated = split(line, ':');
    if (colonSeparated.size() >= 2) {
      string key = trim(colonSeparated[0]);
      string value = trim(colonSeparated[1]);
      if (key == "start") {
        start = atof(value.c_str());
      } else if (key == "duration") {
        duration = atof(value.c_str());
      } else if (key == "steps") {
        steps = atof(value.c_str());
      } else if (key == "variables") {
        auto commaSeparated = split(value, ',');
        for (auto i = 0; i < commaSeparated.size(); i++) {
          variables.push_back(trim(commaSeparated[i]));
        }
      } else if (key == "absolute") {
        absolute = atof(value.c_str());
      } else if (key == "relative") {
        absolute = atof(value.c_str());
      } else if (key == "amount") {
        auto commaSeparated = split(value, ',');
        for (auto i = 0; i < commaSeparated.size(); i++) {
          amount.push_back(trim(commaSeparated[i]));
        }
      } else if (key == "concentration") {
        auto commaSeparated = split(value, ',');
        for (auto i = 0; i < commaSeparated.size(); i++) {
          concentration.push_back(trim(commaSeparated[i]));
        }
      }
    }
  }

  ifs.close();

  auto stepInterval = duration / steps;
  vector<OutputField> outputFields;
  for (auto i = 0; i < variables.size(); i++) {
    auto it = find(amount.begin(), amount.end(), variables[i]);
    if (it != amount.end()) {
      outputFields.push_back(OutputField(variables[i], OutputType::AMOUNT));
    } else {
      outputFields.push_back(OutputField(variables[i], OutputType::CONCENTRATION));
    }
  }

  /*
  cout << "CONFIGURATION" << endl;
  cout << "case dir: " << caseDirPath << endl;
  cout << "model number: " << modelNumber << endl;
  cout << "setting file: " << settingFilename << endl;
  cout << "start: " << start << endl;
  cout << "duration: " << duration << endl;
  cout << "stepInterval: " << stepInterval << endl;
  cout << "variables: " << endl;
  for (auto i = 0; i < outputFields.size(); i++) {
    cout << "  - " << outputFields[i] << endl;
  }
  cout << "absolute: " << absolute << endl;
  cout << "relative: " << relative << endl;
   */

  return RunConfiguration(start, duration, stepInterval, outputFields, absolute, relative);
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
