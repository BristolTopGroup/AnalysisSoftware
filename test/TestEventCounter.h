#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/EventCounter.h"

static const unsigned int dimension1 = 4;
static const unsigned int dimension2 = 2;
static const unsigned int dimension3 = 5;

struct TestEventCounter {

public:
	TestEventCounter() {
	}

	void testStandardCounterConstructor() {
		Counter standardCounter = Counter();
		ASSERT_EQUAL(0, standardCounter.getSizeOfFirstDimension());
		ASSERT_EQUAL(0, standardCounter.getSizeOfSecondDimension());
		ASSERT_EQUAL(0, standardCounter.getSizeOfThirdDimension());
	}

	void testCounterConstructor() {
		Counter counter = Counter(dimension1, dimension2, dimension3);
		ASSERT_EQUAL(dimension1, counter.getSizeOfFirstDimension());
		ASSERT_EQUAL(dimension2, counter.getSizeOfSecondDimension());
		ASSERT_EQUAL(dimension3, counter.getSizeOfThirdDimension());
	}
	void testIncereaseCoutner() {
		Counter counter = Counter(dimension1, dimension2, dimension3);
		counter.increase(0, 0, 0, 2.4);
		ASSERT_EQUAL(1, counter.getEntries(0, 0, 0));
	}

	void testIncreaseWeightedCounter() {
		Counter counter = Counter(dimension1, dimension2, dimension3);
		counter.increase(0, 0, 0, 2.4);
		ASSERT_EQUAL_DELTA(2.4, counter.getWeightedEntries(0, 0, 0), 0.1);
	}
};

extern cute::suite make_suite_TestEventCounter() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestEventCounter, testStandardCounterConstructor));
	s.push_back(CUTE_SMEMFUN(TestEventCounter, testCounterConstructor));
	s.push_back(CUTE_SMEMFUN(TestEventCounter, testIncereaseCoutner));
	s.push_back(CUTE_SMEMFUN(TestEventCounter, testIncreaseWeightedCounter));

	return s;
}
