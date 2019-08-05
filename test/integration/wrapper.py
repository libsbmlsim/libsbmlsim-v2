#!/usr/bin/env python
import sys
import os

# check args
if (len(sys.argv) < 6):
  print('Usage: %s test_case_dir num out_dir level version' % sys.argv[0])
  print('   ex: %s ~/.test-suite/cases/semantic 00001 /tmp/output 3 1' % sys.argv[0])
  sys.exit(0)

# parse args
test_case_dir = sys.argv[1]
num = sys.argv[2]
out_dir = sys.argv[3]
level = sys.argv[4]
version = sys.argv[5]

# directories and paths 
## simulator
fdir = os.path.dirname(os.path.abspath(__file__))
ppdir = os.path.dirname(os.path.dirname(fdir))  # same as '../../fdir'
sim = ppdir + '/test/integration/testintegrator'
## sbml and settings
case_dir = test_case_dir + '/' + num
sbml = num + '-sbml-l' + level + 'v' + version + '.xml'
sbml_path = case_dir + '/' + sbml
setting_file = num + '-settings.txt'
setting_path = case_dir + '/' + setting_file

# parse %n-settings.txt file
with open(setting_path, 'r') as f:
  data = f.readlines()
  for line in data:
    words = line.split(":")
    if (words[0] == 'duration'):
      duration = words[1].strip()
    if (words[0] == 'steps'):
      steps = words[1].strip()
    if (words[0] == 'variables'):
      variables = words[1].strip()
    if (words[0] == 'amount'):
      amount = words[1].strip()
    if (words[0] == 'concentration'):
      concentration = words[1].strip()

opts = '-t %s -s %s -v "%s" -a "%s" -c "%s"' \
        % (duration, steps, variables, amount, concentration)

#commandline = sim + ' ' + opts + ' ' + sbml_path + " | sed -e 's/ /,/g' > " + out_dir + '/' + num + '.csv'
commandline = sim + ' ' + opts + ' ' + sbml_path + " > " + out_dir + '/' + num + '.csv'
os.system(commandline)
