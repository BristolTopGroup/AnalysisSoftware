#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/RecoObjects/Vertex.h"

using namespace BAT;

struct TestVertex {
private:
    double goodVertexMaximalAbsoluteRho;
    double goodVertexMinimalNumberOfDegreesOfFreedom;
    double goodVertexMaximalAbsoluteZPosition;

    Vertex goodVertex;
    Vertex badNDOFVertex;
    Vertex badZPositionVertex;
    Vertex badRhoVertex;
    Vertex fakeVertex;
    float maxGoodRho, maxGoodAbsoluteZ;
    unsigned int minGoodNDOF;

public:
    TestVertex() :
        goodVertexMaximalAbsoluteRho(2.0),
        goodVertexMinimalNumberOfDegreesOfFreedom(4),
        goodVertexMaximalAbsoluteZPosition(15.),
        goodVertex(),
        badNDOFVertex(),
        badZPositionVertex(),
        badRhoVertex(),
        fakeVertex() {
        setGoodVertexCuts();
        setGoodVertex();
        setBadNDOFVertex();
        setBadZPositionVertex();
        setBadRhoVertex();
        setFakeVertex();
    }
private:
    void setGoodVertexCuts() {
        maxGoodAbsoluteZ = goodVertexMaximalAbsoluteZPosition;
        maxGoodRho = goodVertexMaximalAbsoluteRho - 0.1 * goodVertexMaximalAbsoluteRho;
        minGoodNDOF = goodVertexMinimalNumberOfDegreesOfFreedom + 1;
    }

    void setGoodVertex() {
        goodVertex.setDegreesOfFreedom(minGoodNDOF);
        goodVertex.setFake(false);
        goodVertex.setRho(maxGoodRho);
        goodVertex.setZPosition(maxGoodAbsoluteZ);
    }

    void setBadNDOFVertex() {
        badNDOFVertex.setDegreesOfFreedom(minGoodNDOF - 1);
        badNDOFVertex.setFake(false);
        badNDOFVertex.setRho(maxGoodAbsoluteZ);
        badNDOFVertex.setZPosition(maxGoodAbsoluteZ);
    }

    void setBadZPositionVertex() {
        badZPositionVertex.setDegreesOfFreedom(minGoodNDOF);
        badZPositionVertex.setFake(false);
        badZPositionVertex.setRho(maxGoodAbsoluteZ);
        badZPositionVertex.setZPosition(maxGoodAbsoluteZ + 1);
    }

    void setBadRhoVertex() {
        badRhoVertex.setDegreesOfFreedom(minGoodNDOF);
        badRhoVertex.setFake(false);
        badRhoVertex.setRho(maxGoodRho + 2);
        badRhoVertex.setZPosition(maxGoodAbsoluteZ);
    }

    void setFakeVertex() {
        fakeVertex.setDegreesOfFreedom(minGoodNDOF);
        fakeVertex.setFake(true);
        fakeVertex.setRho(maxGoodRho);
        fakeVertex.setZPosition(maxGoodAbsoluteZ);
    }
public:
    void testSetDegreesOfFreedom() {
        goodVertex.setDegreesOfFreedom(111);
        ASSERT_EQUAL(111, goodVertex.ndof());
    }

    void testSetFake() {
        ASSERT(goodVertex.isFake() == false);
        goodVertex.setFake(true);
        ASSERT(goodVertex.isFake());
    }

    void testSetPositiveRho() {
        goodVertex.setRho(222.);
        ASSERT_EQUAL(222., goodVertex.absoluteRho());
    }

    void testSetNegativeRho() {
        goodVertex.setRho(-222.);
        ASSERT_EQUAL(222., goodVertex.absoluteRho());
    }

    void testSetPositiveZ() {
        goodVertex.setZPosition(333.);
        ASSERT_EQUAL(333., goodVertex.absoluteZPosition());
    }

    void testSetNegativeZ() {
        goodVertex.setZPosition(-333.);
        ASSERT_EQUAL(333., goodVertex.absoluteZPosition());
    }

    void testGoodVertex() {
        ASSERT(goodVertex.isGood());
    }


    void testBadNDOFVertex() {
        ASSERT(badNDOFVertex.isGood() == false);
    }

    void testBadZpositionVertex() {
        ASSERT(badZPositionVertex.isGood() == false);
    }

    void testBadRhoVertex() {
        ASSERT(badRhoVertex.isGood() == false);
    }

    void testFakeVertex() {
        ASSERT(fakeVertex.isGood() == false);
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
