//
// Created by thomas on 2/20/18.
//

#include "gtest/gtest.h"
#include "../src/Vector2.h"

TEST(Vector2, arithmeticOperators) {
    Vector2 v1{1, 1};
    Vector2 v2{1, 2};
    Vector2 temp;

    temp = v1 + v2;
    ASSERT_DOUBLE_EQ(temp.getX(), 2.0);
    ASSERT_DOUBLE_EQ(temp.getY(), 3.0);
    temp = v1 - v2;
    ASSERT_DOUBLE_EQ(temp.getX(), 0.0);
    ASSERT_DOUBLE_EQ(temp.getY(),-1.0);
    temp = v1 * v2;
    ASSERT_DOUBLE_EQ(temp.getX(), 1.0);
    ASSERT_DOUBLE_EQ(temp.getY(), 2.0);
    temp = v1 / v2;
    ASSERT_DOUBLE_EQ(temp.getX(), 1.0);
    ASSERT_DOUBLE_EQ(temp.getY(), 0.5);

    temp = v1 * 3;
    ASSERT_DOUBLE_EQ(temp.getX(), 3.0);
    ASSERT_DOUBLE_EQ(temp.getY(), 3.0);
    temp = v1 / 2;
    ASSERT_DOUBLE_EQ(temp.getX(), 0.5);
    ASSERT_DOUBLE_EQ(temp.getY(), 0.5);
}