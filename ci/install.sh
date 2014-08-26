#!/bin/bash
# This script is meant to be called by the "install" step defined in
# .travis.yml. See http://docs.travis-ci.com/ for more details.
# The behavior of the script is controlled by environment variabled defined
# in the .travis.yml in the top level folder of the project.

set -e

# Install a ROOT binary that we custom-built in a 64-bit Ubuntu VM
# for the correct python / ROOT version
time wget --no-check-certificate https://copy.com/s3BcYu1drmZa/ci/root_builds/root_v${ROOT}_python_2.7.tar.gz
time tar zxf root_v${ROOT}_python_2.7.tar.gz
mv root_v${ROOT}_python_2.7 root
source root/bin/thisroot.sh

# test ROOT install
# Check if ROOT
root -l -q

# setup newer compilers ( we need gcc >= 4.7 for c++11
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50;
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50;
sudo update-alternatives --set gcc /usr/bin/gcc-4.8; sudo update-alternatives --set g++ /usr/bin/g++-4.8;

cmake CMakelists.txt
make
