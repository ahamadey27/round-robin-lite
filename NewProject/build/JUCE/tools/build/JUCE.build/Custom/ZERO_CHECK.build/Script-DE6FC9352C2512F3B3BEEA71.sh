#!/bin/sh
set -e
if test "$CONFIGURATION" = "Custom"; then :
  cd /Users/alex/Documents/Github/round-robin-lite/NewProject/build/JUCE/tools
  make -f /Users/alex/Documents/Github/round-robin-lite/NewProject/build/JUCE/tools/CMakeScripts/ReRunCMake.make
fi

