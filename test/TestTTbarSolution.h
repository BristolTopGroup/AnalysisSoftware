/*
 * TestTH1Collection.h
 *
 *  Created on: 9 Aug 2010
 *      Author: kreczko
 */

#include "cute/cute_suite.h"
#include "../interface/TtbarHypothesis.h"

using namespace BAT;

struct TestTTbarSolution {
private:
    std::vector<TtbarHypothesisPointer> solutions;
    compare_totalChi2 compareSolutions;
public:
    TestTTbarSolution() :
       solutions(),
       compareSolutions() {
        TtbarHypothesisPointer solution1(new TtbarHypothesis());
        TtbarHypothesisPointer solution2(new TtbarHypothesis());
        TtbarHypothesisPointer solution3(new TtbarHypothesis());
        TtbarHypothesisPointer solution4(new TtbarHypothesis());
        solution1->totalChi2 = 1;
        solution2->totalChi2 = 2;
        solution3->totalChi2 = 3;
        solution4->totalChi2 = 4;
        solutions.push_back(solution3);
        solutions.push_back(solution2);
        solutions.push_back(solution1);
        solutions.push_back(solution4);
        std::sort(solutions.begin(), solutions.end(), compareSolutions);
    }

    ~TestTTbarSolution() {
    }

    void testSorting() {
        for(unsigned int index = 0; index < solutions.size(); ++ index){
            ASSERT_EQUAL_DELTA(index + 1, solutions.at(index)->totalChi2, 0.0001);
        }
    }

};

extern cute::suite make_suite_TestTTbarSolution() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestTTbarSolution, testSorting));

    return s;
}

