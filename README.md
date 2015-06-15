AnalysisSoftware
================

Analysis software for TTbar+X differential cross section analysis (TOP-12-042)

## General Recipe

To setup the stand-alone version of the code:

```
# this section will be revisited, please use CMSSW setup for now
# get the code from the repository
git clone https://github.com/BristolTopGroup/AnalysisSoftware.git AnalysisSoftware
cd AnalysisSoftware

# on DICE need to source ROOT:
# source /software/root/v5.32.00/bin/thisroot.sh

# create the makefile using cmake (install it if you don't have it: https://github.com/Kitware/CMake)
# minimal cmake version 2.6.0 according to CMakeLists.txt
cmake CMakeLists.txt
#on DICE use cmake28
# cmake28 CMakeLists.txt

# for older versions of boost, e.g. on DICE, you will need to
#sed -i 's/lib64\/lib64/lib64/g' ./CMakeFiles/AnalysisSoftware.dir/build.make
#sed -i 's/lib64\/lib64/lib64/g' ./CMakeFiles/AnalysisSoftware.dir/link.txt
# as the library path is wrong (contains lib64/lib64)

# compile and build
make -j24

# adjust the toolsFolder environment variable
export toolsFolder=$PWD/

# test run the code:
nohup ./AnalysisSoftware python/test_cfg.py &> test.log &
```


To setup the code using CMSSW:

```
# on soolin (and other scientific linux 6 machines)
export SCRAM_ARCH=slc6_amd64_gcc491
# This version comes with ROOT 6.02/05
scram p -n CMSSW_7_5_0_pre5_AS CMSSW_7_5_0_pre5
cd CMSSW_7_5_0_pre4_AS/src/
cmsenv
# initialise CMS git
git cms-init

# get the code from the repository
git clone https://github.com/BristolTopGroup/AnalysisSoftware.git BristolAnalysis/Tools
cd BristolAnalysis/Tools

# get submodules:
git submodule init && git submodule update
cd -
ln -s BristolAnalysis/Tools/external/TopQuarkAnalysis TopQuarkAnalysis
# compile
scram b -j2

hash -r #or rehash in case that BAT cannot be found

# test run the code:
#../tmp/slc6_amd64_gcc491/src/BristolAnalysis/Tools/test/BAT_Tests/BAT_Tests
nohup BAT BristolAnalysis/Tools/python/test_cfg.py &> test.log &
```

## CLHEP
This is not needed for CMSSW setup
```
mkdir /software
cd /software
wget http://proj-clhep.web.cern.ch/proj-clhep/DISTRIBUTION/tarFiles/clhep-2.2.0.8.tgz
tar xzf clhep-2.2.0.8.tgz
mkdir /software/2.2.0.8/build
cd /software/2.2.0.8/build
cmake /software/2.2.0.8/CLHEP
make jobs=2
make install
```


## Eclipse
If you want like to use Eclipse for the development of the code, initial
project files are provided in the ```eclipse/``` folder. You can copy them into
the main directory (example for OS X):
```
# in AnalysisSoftware folder
cp eclipse/.project.osx .project
cp eclipse/.cproject.osx .cproject
cp eclipse/.pydevproject.osx .pydevproject
```

### Code Style
A code style prescription is also available in ```eclipse/Bristol_C++_CodeStyle.xml```.
You should import it ```under Preferences->C/C++->Code Style->Formatter```

# Notes
More information can be found at [Bristol Analysis Tools twiki page](https://twiki.cern.ch/twiki/bin/view/CMS/BristolAnalysisTools)

## Special notes for OS X users
### boost & boost-python
Install boost and boost python using [homebrew](http://brew.sh/):
```
brew install boost boost-python --c++11
```


### Python header issue
You might find yourself running into a compilation problem as described in issue #115.
The solution is to replace (in ```/System/Library/Frameworks/Python.framework/Headers/pyport.h```)
```
#undef isalnum
#define isalnum(c) iswalnum(btowc(c))
#ifdef isalpha
#define isalpha(c) iswalpha(btowc(c))
#undef islower
#define islower(c) iswlower(btowc(c))
#undef isspace
#define isspace(c) iswspace(btowc(c))
#undef isupper
#define isupper(c) iswupper(btowc(c))
#undef tolower
#define tolower(c) towlower(btowc(c))
#undef toupper
#define toupper(c) towupper(btowc(c))
```
with
```
#ifdef isalnum
#undef isalnum
#define isalnum(c) iswalnum(btowc(c))
#endif
#ifdef isalpha
#undef isalpha
#define isalpha(c) iswalpha(btowc(c))
#endif
#ifdef islower
#undef islower
#define islower(c) iswlower(btowc(c))
#endif
#ifdef isspace
#undef isspace
#define isspace(c) iswspace(btowc(c))
#endif
#ifdef isupper
#undef isupper
#define isupper(c) iswupper(btowc(c))
#endif
#ifdef tolower
#undef tolower
#define tolower(c) towlower(btowc(c))
#endif
#ifdef toupper
#undef toupper
#define toupper(c) towupper(btowc(c))
#endif
```
(from https://codereview.appspot.com/179049/patch/1/2)

## HTCondor
Once the software is set up one can use HTCondor to submit jobs to a cluster.
Instructions for this can be found in [condor/README.md](condor/README.md)

# Bugs
Please report any problems on our [issues page](https://github.com/BristolTopGroup/AnalysisSoftware/issues).

# Run tests
## Boost unit tests 
Running all tests
```
make test
# or
./AnalysisSoftware_test --log_level=message
```
Running a specific test suite:
```
./AnalysisSoftware_test --log_level=message --run_test=DataTypeTests
```

Running a specific test:
```
./AnalysisSoftware_test --log_level=message --run_test=DataTypeTests/NoAmbiguousMappingHard
```


## Python unit tests
Python tests require ```nose``` to be installed:
```
pip install nose --upgrade
```

You can then run the tests with:
```
time nosetests -v test/*.py
```
