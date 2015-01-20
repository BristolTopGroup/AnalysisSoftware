//#define BOOST_TEST_MODULE DataTypeTests
#include <boost/test/unit_test.hpp>
#include "../interface/DataTypes.h"
#include <string>
#include <boost/algorithm/string/predicate.hpp>

BOOST_AUTO_TEST_SUITE(DataTypeTests)
BOOST_AUTO_TEST_CASE( NoSimilarDataTypes ) {
	for (auto i = 0; i < BAT::DataType::names.size(); ++i) {
		std::string name1 = BAT::DataType::names.at(i);
		for (auto j = 0; j < BAT::DataType::names.size(); ++j) {
			std::string name2 = BAT::DataType::names.at(j);
			if (name1 != name2) {
				// there should be no overlap with sub-strings
				BOOST_REQUIRE_MESSAGE(!boost::starts_with(name1, name2), name1 << " and " << name2 << " are similar");
			}
		}
	}
}

BOOST_AUTO_TEST_CASE( AllDataTypesHaveMapping ) {
	BOOST_REQUIRE(BAT::DataType::NUMBER_OF_DATA_TYPES == BAT::DataType::names.size());

}
BOOST_AUTO_TEST_SUITE_END( )
