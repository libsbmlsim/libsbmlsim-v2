#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Batch script for SBML test runner.
# This script requires testsuite-core.jar to be installed on your system.
# 
# Last modified: Sun, 13 Nov 2016 06:46:09 +0900
# 

import sys
import os
import shutil
import subprocess
import re

# color class
class bcolors:
  red = '\033[91m'
  green = '\033[92m'
  blue = '\033[94m'
  endc = '\033[0m'
  bold = '\033[1m'
  ul = '\033[4m'

# config
# wrapperName should be defined in your ~/.testsuite.xml
wrapperName='libsbmlsim-v2'
deplibs='simple-xml-2.6.4.jar'
scriptdir=sys.path[0]
guidir=scriptdir + '/testsuite/src/front-ends/standalone/testsuite-ui/dist'
jardir=scriptdir + '/testsuite/src/test-runner/testsuite-core/dist'
libdir=scriptdir + '/testsuite/src/test-runner/testsuite-core/libs'

# check args
if (len(sys.argv) < 2):
  print('%s: command line SBML test runner' % sys.argv[0])
  print('Usage: %s [-g|--gui] test-range' % sys.argv[0])
  print('   ex: %s 1-1000' % sys.argv[0])
  sys.exit(0)

# parse args
## launch GUI
if (sys.argv[1] == '-g' or sys.argv[1] == '--gui'):
  cmd = 'java -XstartOnFirstThread -jar ' + guidir + '/sbmltestrunner.jar'
  subprocess.Popen(cmd.strip().split(' '))
  sys.exit(0)

## args for test range
test_range = sys.argv[1]

# check libs
if (not os.path.isfile(jardir + '/' + deplibs)):
  print('Copy %s to %s' % (deplibs, jardir))
  shutil.copy2(libdir + '/'  + deplibs, jardir)

# run tests
os.chdir(jardir)
cmd = 'java -cp ' + deplibs +':. -jar testsuite-core.jar -r ' + wrapperName + ' ' + test_range + ' | paste -d " " - - '
array = subprocess.check_output(cmd, shell=True).splitlines()

# check status
failed = False
if len(array) <= 0:
  failed = False
for line in array:
  line = line.decode() # python3 compatible
  line = line.replace('NoMatch', bcolors.bold + bcolors.red + 'NoMatch' + bcolors.endc) 
  line = line.replace(' Match',  bcolors.bold + bcolors.green + ' Match' + bcolors.endc) 
  line = line.replace('CannotSolve',  bcolors.bold + bcolors.blue + 'CannotSolve' + bcolors.endc) 
  print(line)
  if re.match('.*NoMatch.*', line):
    failed = True

if failed:
  sys.exit(1)
else:
  sys.exit(0)
