#!/bin/bash
# This script is meant to be called by the "before_install" step defined in
# .travis.yml. See http://docs.travis-ci.com/ for more details.
# The behavior of the script is controlled by environment variabled defined
# in the .travis.yml in the top level folder of the project.

set -e
sudo add-apt-repository --yes ppa:kalakris/cmake
# add repositories for gcc 4.8 and clang $CLANG_VERSION (set in .travis.yml)
sudo add-apt-repository --yes ppa:ubuntu-toolchain-r/test
sudo add-apt-repository --yes 'deb http://llvm.org/apt/precise/ llvm-toolchain-precise main'
sudo add-apt-repository --yes 'deb http://ppa.launchpad.net/boost-latest/ppa/ubuntu precise main'
wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -
# Needed because sometimes travis' repositories get out of date
sudo apt-get update -qq
# Install the dependencies we need
time sudo apt-get -qq install cmake clang-${CLANG_VERSION} libclang-${CLANG_VERSION}-dev gcc-4.8 g++-4.8 boost1.55
