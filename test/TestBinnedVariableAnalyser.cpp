#include <boost/test/unit_test.hpp>
#include <vector>

#include "../interface/Analysers/Binned_variable_analyser.h"
#include "../interface/HistHelpers/HistogramManager.h"

using namespace BAT;

struct TestSetup {
	TestSetup() :
					histMan(new HistogramManager()),
					analyser(new Binned_variable_analyser(histMan, "TestBinnedVariableAnalyser")),
					zero_start_var(),
					non_zero_start_var(),
					fit_variable("fit_var", 20, 0.0, 100.),
					zero_var_bins(),
					non_zero_var_bins() {
		boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes;
		for (unsigned int i = 0; i < DataType::NUMBER_OF_DATA_TYPES; ++i)
			seenDataTypes.at(i) = false;
		seenDataTypes.at(DataType::SingleElectron) = true;
		histMan->prepareForSeenDataTypes(seenDataTypes);
		histMan->setCurrentDataType(DataType::SingleElectron);
		zero_var_bins.push_back(0);
		zero_var_bins.push_back(1);
		zero_var_bins.push_back(5);
		zero_var_bins.push_back(10);

		non_zero_var_bins.push_back(1);
		non_zero_var_bins.push_back(5);
		non_zero_var_bins.push_back(10);

		zero_start_var = Variable("zero_start_var", zero_var_bins);
		non_zero_start_var = Variable("non_zero_start_var", non_zero_var_bins);

	}

	~TestSetup() {

	}

	HistogramManagerPtr histMan;
	Binned_Variable_analyser_ptr analyser;
	Variable zero_start_var, non_zero_start_var, fit_variable;
	std::vector<double> zero_var_bins, non_zero_var_bins;
};
BOOST_AUTO_TEST_SUITE (TestBinnedVariableAnalyser)
BOOST_FIXTURE_TEST_CASE(test_zero_start_var, TestSetup) {
	TH1F::AddDirectory(false);
	analyser->set_variables(zero_start_var, fit_variable);
	analyser->createHistograms();
	BOOST_CHECK_EQUAL(histMan->size(), zero_var_bins.size());

	// this folder should not exist
	string folder = "TestBinnedVariableAnalyser/" + zero_start_var.name + "_bin_0-0";
	BOOST_CHECK_EQUAL(histMan->hasFolder(folder), false);
	// this should be the first valid bin
	folder = "TestBinnedVariableAnalyser/" + zero_start_var.name + "_bin_0-1";
	BOOST_CHECK_EQUAL(histMan->hasFolder(folder), true);

	// this should go into the first bin
	analyser->analyse(0.5, 3, 1.);
	histMan->setCurrentHistogramFolder(folder);
	// only check further if histograms are created
	BOOST_CHECK_EQUAL(histMan->H1D("fit_var")->Integral(), 1);
}

BOOST_FIXTURE_TEST_CASE(test_non_zero_start_var, TestSetup) {
	analyser->set_variables(non_zero_start_var, fit_variable);
	analyser->createHistograms();
	BOOST_CHECK_EQUAL(histMan->size(), non_zero_var_bins.size());

	// this folder should not exist
	string folder = "TestBinnedVariableAnalyser/" + non_zero_start_var.name + "_bin_0-1";
	BOOST_CHECK_EQUAL(histMan->hasFolder(folder), false);
	// this should be the first valid bin
	folder = "TestBinnedVariableAnalyser/" + non_zero_start_var.name + "_bin_1-5";
	BOOST_CHECK_EQUAL(histMan->hasFolder(folder), true);

	// this should not go anywhere
	analyser->analyse(0.5, 3, 1.);
	histMan->setCurrentHistogramFolder(folder);
	BOOST_CHECK_EQUAL(histMan->H1D("fit_var")->Integral(), 0);

	// but this should
	analyser->analyse(4, 3, 1.);
	histMan->setCurrentHistogramFolder(folder);
	BOOST_CHECK_EQUAL(histMan->H1D("fit_var")->Integral(), 1);
}

BOOST_AUTO_TEST_SUITE_END()

