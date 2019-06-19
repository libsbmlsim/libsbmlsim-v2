#!/bin/sh

if [ $# -lt 1 ]; then
  echo "Usage: ./batchTestPrintFailed.sh start-end (e.g. 1-10)"
  exit 1
fi

./batchTest.py $* | grep -E 'NoMatch|CannotSolve' | cut -d' ' -f6
