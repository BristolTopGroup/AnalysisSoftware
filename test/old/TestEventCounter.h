#ifndef TESTEVENTCOUNTER_h
#define TESTEVENTCOUNTER_h
#include "cute/cute.h"
#include "cute/cute_suite.h"

static const unsigned int dimension1 = 4;
static const unsigned int dimension2 = 2;
static const unsigned int dimension3 = 5;

struct TestEventCounter {

public:
	TestEventCounter();
	void testStandardCounterConstructor();
	void testCounterConstructor();
	void testIncereaseCoutner();
	void testIncreaseWeightedCounter();
};

extern cute::suite make_suite_TestEventCounter();
#endif
