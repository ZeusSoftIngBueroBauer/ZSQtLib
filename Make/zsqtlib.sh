#!/bin/sh
# Environment variables for the ZSQtLib package:
if [ -d /media/sf_LTT/ZSQtLib ]; then
  ZSQTLIB_DIR=/media/sf_LTT/ZSQtLib
else
  exit
fi
export ZSQTLIB_DIR
