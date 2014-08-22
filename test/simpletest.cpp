/*********************************************************************************
 * This file is part of CUTE.
 *
 * CUTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CUTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CUTE.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2007 Peter Sommerlad
 *
 *********************************************************************************/

#include <iostream>
#include "cute_runner.h"
#include "ide_listener.h"
#include "cute.h"

int lifeTheUniverseAndEverything = 42;

void mySimpleTest() {
	ASSERT_EQUAL(lifeTheUniverseAndEverything, 6 * 7);
}

void example1() {
	using namespace std;
	cute::test successfulTest = CUTE(mySimpleTest);
	cute::ide_listener<> lis;
	cute::runner<cute::ide_listener<> > run = cute::makeRunner(lis);
	if (run(successfulTest)) {
		cout << "OK" << endl;
	} else {
		cout << "failed" << endl;
	}
}

void example2() {
	using namespace std;
	cute::ide_listener<> lis;
	cute::makeRunner(lis)(CUTE(mySimpleTest));
}

int anotherTest() {
	ASSERT_EQUAL(42, lifeTheUniverseAndEverything);
	return 0;
}

cute::test tests[] = {
#ifdef __GNUG__
		CUTE(mySimpleTest), mySimpleTest, CUTE(anotherTest)
#else /* for MSVC... */
		CUTE(mySimpleTest)
		,CUTE(mySimpleTest)
		,CUTE(reinterpret_cast<void(*)()>(anotherTest))
#endif
	};

struct ATestFunctor {
	void operator()() {
		ASSERT_EQUAL_DELTA(42.0,
				static_cast<double>(lifeTheUniverseAndEverything), 0.001);
	}
};

void example3() {
	using namespace std;
	cute::ide_listener<> lis;
	cute::runner<cute::ide_listener<> > run = cute::makeRunner(lis);
	cute::suite s(tests, tests + (sizeof(tests) / sizeof(tests[0])));
	s += ATestFunctor();
	cute::test failingTest = cute::test(failingTestWithMessage);
	s += failingTest;
	run(s, "suite");
}

int main() {
	example1();
	example2();
	example3();
}
