#!/bin/bash

set -xe

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
	bash ./setup.sh
else

	brew install cmake qt5 bison gettext ninja python3 || echo
	brew install llvm --with-clang
	bash ./setup.sh
		
fi
	


