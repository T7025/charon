//
// Created by thomas on 2/20/18.
//

#include "gtest/gtest.h"
#include "../src/Body.h"

GTEST_TEST(Body, calcCenterOfMass) {
    Body a{{-1, -1}, {0, 0}, 3};
    Body b{{3, 3}, {0, 0}, 1};
    Position pos1, pos2;
    double mass1, mass2;
    std::tie(pos1, mass1) = a.calcCenterOfMass(b);
    std::tie(pos2, mass2) = b.calcCenterOfMass(a);
    ASSERT_DOUBLE_EQ(pos1.getX(), 0.0);
    ASSERT_DOUBLE_EQ(pos1.getY(), 0.0);
    ASSERT_DOUBLE_EQ(mass1, 4.0);
    ASSERT_EQ(pos1.getX(), pos2.getX());
    ASSERT_EQ(pos1.getY(), pos2.getY());
    ASSERT_EQ(mass1, mass2);
}