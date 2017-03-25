#!/bin/bash


SCRIPTSDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPTSDIR/..



clang-format -style=file -i $(find libchigraph libchigraphdebugger chi chigraphgui test -name "*.cpp" -o -name "*.hpp")

