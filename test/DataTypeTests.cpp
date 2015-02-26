//#define BOOST_TEST_MODULE DataTypeTests
#include <boost/test/unit_test.hpp>
#include "../interface/DataTypes.h"
#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/range/adaptor/reversed.hpp>

using namespace BAT;

BOOST_AUTO_TEST_SUITE(DataTypeTests)

BOOST_AUTO_TEST_CASE( AllDataTypesHaveMapping ) {
	BOOST_REQUIRE(DataType::NUMBER_OF_DATA_TYPES == DataType::names.size());
}

BOOST_AUTO_TEST_CASE( NoAmbiguousMappingSoft ) {
	for (auto i = 0; i < DataType::names.size(); ++i) {
		DataType::value name1 = DataType::getDataType(DataType::names.at(i));
		for (auto j = 0; j < DataType::names.size(); ++j) {
			DataType::value name2 = DataType::getDataType(DataType::names.at(j));
			if (i != j) {
				// there should be no overlap with sub-strings
				BOOST_REQUIRE_MESSAGE(name1 != name2, name1 << " and " << name2 << " are similar");
			}
		}
	}
}

BOOST_AUTO_TEST_CASE( NoAmbiguousMappingHard ) {
	for (auto i = (DataType::names.size()); i-- > 0;) {
		DataType::names.at(0);
		DataType::value name1 = DataType::getDataType(DataType::names.at(i));
		for (auto j = 0; j < DataType::names.size(); ++j) {
			DataType::value name2 = DataType::getDataType(DataType::names.at(j));
			if (i != j) {
				// there should be no overlap with sub-strings
				BOOST_REQUIRE_MESSAGE(name1 != name2, name1 << " and " << name2 << " are similar");
			}
		}
	}
}
BOOST_AUTO_TEST_SUITE_END( )
