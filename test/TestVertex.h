#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/Vertex.h"
#include "TestObjectFactory.h"

using namespace BAT;

struct TestVertex {
private:

    VertexPointer goodVertex;
    VertexPointer badNDOFVertex;
    VertexPointer badZPositionVertex;
    VertexPointer badRhoVertex;
    VertexPointer fakeVertex;

public:
    TestVertex() :
        goodVertex(TestObjectFactory::goodVertex()),
        badNDOFVertex(TestObjectFactory::badNDOFVertex()),
        badZPositionVertex(TestObjectFactory::badZPositionVertex()),
        badRhoVertex(TestObjectFactory::badRhoVertex()),
        fakeVertex(TestObjectFactory::badFakeVertex()) {
    }

public:
    void testSetDegreesOfFreedom() {
        goodVertex->setDegreesOfFreedom(111);
        ASSERT_EQUAL(111, goodVertex->ndof());
    }

    void testSetFake() {
        ASSERT(goodVertex->isFake() == false);
        goodVertex->setFake(true);
        ASSERT(goodVertex->isFake());
    }

    void testSetPositiveRho() {
        goodVertex->setRho(222.);
        ASSERT_EQUAL(222., goodVertex->absoluteRho());
    }

    void testSetNegativeRho() {
        goodVertex->setRho(-222.);
        ASSERT_EQUAL(222., goodVertex->absoluteRho());
    }

    void testSetPositiveZ() {
        goodVertex->setZPosition(333.);
        ASSERT_EQUAL(333., goodVertex->absoluteZPosition());
    }

    void testSetNegativeZ() {
        goodVertex->setZPosition(-333.);
        ASSERT_EQUAL(333., goodVertex->absoluteZPosition());
    }

    /**
     * The following are not really necessary since they are tested for in the TestObjectFactory
     */
    void testGoodVertex() {
        ASSERT(goodVertex->isGood());
    }

    void testBadNDOFVertex() {
        ASSERT(badNDOFVertex->isGood() == false);
    }

    void testBadZpositionVertex() {
        ASSERT(badZPositionVertex->isGood() == false);
    }

    void testBadRhoVertex() {
        ASSERT(badRhoVertex->isGood() == false);
    }

    void testFakeVertex() {
        ASSERT(fakeVertex->isGood() == false);
    }
};

cute::suite make_suite_TestVertex() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestVertex, testSetDegreesOfFreedom));
    s.push_back(CUTE_SMEMFUN(TestVertex, testSetFake));
    s.push_back(CUTE_SMEMFUN(TestVertex, testSetPositiveRho));
    s.push_back(CUTE_SMEMFUN(TestVertex, testSetNegativeRho));
    s.push_back(CUTE_SMEMFUN(TestVertex, testSetPositiveZ));
    s.push_back(CUTE_SMEMFUN(TestVertex, testSetNegativeZ));

    s.push_back(CUTE_SMEMFUN(TestVertex, testGoodVertex));
    s.push_back(CUTE_SMEMFUN(TestVertex, testBadNDOFVertex));
    s.push_back(CUTE_SMEMFUN(TestVertex, testBadZpositionVertex));
    s.push_back(CUTE_SMEMFUN(TestVertex, testBadRhoVertex));
    s.push_back(CUTE_SMEMFUN(TestVertex, testFakeVertex));

    return s;
}
