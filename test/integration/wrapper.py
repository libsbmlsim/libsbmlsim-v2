#!/usr/bin/env python
import sys
import os

# check args
if (len(sys.argv) < 6):
  print 'Usage: ' + sys.argv[0] + ' test_case_dir num out_dir level version'
  print '   ex: ' + sys.argv[0] + ' ~/.test-suite/cases/semantic 00001 /tmp/output 3 1'
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
sim = ppdir + '/build/test/integration/testintegrator'
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

opts = '-t ' + duration + ' -d ' + str(float(duration)/float(steps))

#commandline = sim + ' ' + opts + ' ' + sbml_path + " | sed -e 's/ /,/g' > " + out_dir + '/' + num + '.csv'
commandline = sim + ' ' + opts + ' ' + sbml_path + " > " + out_dir + '/' + num + '.csv'
os.system(commandline)
