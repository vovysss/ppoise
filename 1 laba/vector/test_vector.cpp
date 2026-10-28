
#include <gtest/gtest.h>
#include "vector.h"
#include <cmath>
#include <stdexcept>

TEST(VectorTest, DefaultConstructor) {
    Vector v;
    EXPECT_DOUBLE_EQ(v.length(), 0.0);
}

TEST(VectorTest, ParamConstructorAndGetters) {
    Vector v(1.0, 2.0, 3.0, 4.0, 6.0, 8.0);
    EXPECT_DOUBLE_EQ(v.getX1(), 1.0);
    EXPECT_DOUBLE_EQ(v.getY1(), 2.0);
    EXPECT_DOUBLE_EQ(v.getZ1(), 3.0);
    EXPECT_DOUBLE_EQ(v.getX2(), 4.0);
    EXPECT_DOUBLE_EQ(v.getY2(), 6.0);
    EXPECT_DOUBLE_EQ(v.getZ2(), 8.0);
}

TEST(VectorTest, Length) {
    Vector v(0, 0, 0, 3, 4, 0);
    EXPECT_DOUBLE_EQ(v.length(), 5.0);
}

TEST(VectorTest, Addition) {
    Vector v1(0, 0, 0, 1, 2, 3);
    Vector v2(0, 0, 0, 4, 5, 6);
    Vector v3 = v1 + v2;
    EXPECT_DOUBLE_EQ(v3.getX2() - v3.getX1(), 5.0);
    EXPECT_DOUBLE_EQ(v3.getY2() - v3.getY1(), 7.0);
    EXPECT_DOUBLE_EQ(v3.getZ2() - v3.getZ1(), 9.0);
}

TEST(VectorTest, CrossProduct) {
    Vector i(0, 0, 0, 1, 0, 0);
    Vector j(0, 0, 0, 0, 1, 0);
    Vector k = i * j;
    EXPECT_DOUBLE_EQ(k.getX2() - k.getX1(), 0.0);
    EXPECT_DOUBLE_EQ(k.getY2() - k.getY1(), 0.0);
    EXPECT_DOUBLE_EQ(k.getZ2() - k.getZ1(), 1.0);
}

TEST(VectorTest, ScalarMultiplication) {
    Vector v(0, 0, 0, 1, 2, 3);
    Vector v2 = v * 2.0;
    EXPECT_DOUBLE_EQ(v2.getX2() - v2.getX1(), 2.0);
    EXPECT_DOUBLE_EQ(v2.getY2() - v2.getY1(), 4.0);
    EXPECT_DOUBLE_EQ(v2.getZ2() - v2.getZ1(), 6.0);
}

TEST(VectorTest, DivisionByZeroException) {
    Vector v(0, 0, 0, 1, 2, 3);
    EXPECT_THROW(v / 0.0, std::invalid_argument);
}

TEST(VectorTest, AngleCosine) {
    Vector v1(0, 0, 0, 1, 0, 0);
    Vector v2(0, 0, 0, 2, 0, 0);
    EXPECT_NEAR(v1 ^ v2, 1.0, 1e-6);

    Vector v3(0, 0, 0, 0, 1, 0);
    EXPECT_NEAR(v1 ^ v3, 0.0, 1e-6);
}

TEST(VectorTest, LengthComparison) {
    Vector v1(0, 0, 0, 3, 4, 0); 
    Vector v2(0, 0, 0, 0, 0, 6); 
    EXPECT_TRUE(v1 < v2);
    EXPECT_TRUE(v2 > v1);
    EXPECT_TRUE(v1 != v2);
}

