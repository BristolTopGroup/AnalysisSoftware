This is the contribute.md of our project. Great to have you here. Here are a few ways you can help make this project better!

# Contributing to AnalysisSoftware

## Team members
Our core development team consists of the members: @jjacob @EmyrClement @kreczko

## Running tests
After compiling AnalysisSoftware the most simple way to run the tests is
```shell
make test
```
or to run the executable that is produced
```shell
./AnalysisSoftware_test
```
The latter is particularly useful if you want to run a subset of tests.
As an example
```shell
 ./AnalysisSoftware_test --run_test=DataTypeTests/AllDataTypesHaveMapping
```
would run only the ```AllDataTypesHaveMapping``` test case in the ```DataTypeTests``` test suite.

## Adding new features
When adding a new feature, remember to add tests for it as well.

### Adding a new dataset
Adding a new dataset consists of the following steps:
 * in ```interface/DataTypes.h``` add the name to the ```BAT::DataType::value```
 * in ```interface/DataTypes.h``` add the prefix (input & output files) to the ```BAT::DataType::names```
 * in ```python/DataSetInfo_*``` add the dictionary entry: ```datasetInfo['<new data set name>'] = {"cross-section": 172, "NumberOfProcessedEvents":17100187}```

*Important note*: The data type is recognised by the start of the input file name. 
If multiple datasets start with the same set of letters, this will lead to ambiguities (e.g. 'myset' & 'myset123')

