#include <gtest/gtest.h>
#include "rectangle.h"

TEST(RectangleTest, ConstructorAndArea) {
    Rectangle r(0, 0, 4, 3); 
    EXPECT_EQ(r.getWidth(), 4);
    EXPECT_EQ(r.getHeight(), 3);
    EXPECT_EQ(r.area(), 12);
}

TEST(RectangleTest, Normalization) {
    Rectangle r(4, 3, 0, 0); 
    EXPECT_EQ(r.getWidth(), 4);
    EXPECT_EQ(r.getHeight(), 3);
}

TEST(RectangleTest, MoveAndResize) {
    Rectangle r(0, 0, 2, 2);
    r.move(3, -1);
    
    int x, y;
    r.getBottomLeft(x, y);
    EXPECT_EQ(x, 3);
    EXPECT_EQ(y, -1);

    r.resize(5, 4);
    EXPECT_EQ(r.getWidth(), 5);
    EXPECT_EQ(r.getHeight(), 4);
}

TEST(RectangleTest, Increments) {
    Rectangle r(0, 0, 2, 2); 
    ++r; 
    EXPECT_EQ(r.getWidth(), 3);
    EXPECT_EQ(r.getHeight(), 3);

    r--; 
    EXPECT_EQ(r.getWidth(), 2);
    EXPECT_EQ(r.getHeight(), 2);
}

TEST(RectangleTest, Intersection) {
    Rectangle r1(0, 0, 4, 4);
    Rectangle r2(2, 2, 6, 6);
    
    EXPECT_TRUE(r1.intersects(r2));

    Rectangle r3 = r1 - r2; 
    EXPECT_EQ(r3.getWidth(), 2);
    EXPECT_EQ(r3.getHeight(), 2);
}


TEST(RectangleTest, Union) {
    Rectangle r1(0, 0, 2, 2);
    Rectangle r2(3, 3, 5, 5);
    
    Rectangle r3 = r1 + r2; 
    
    int x1, y1, x2, y2;
    r3.getBottomLeft(x1, y1);
    r3.getTopRight(x2, y2);
    
    EXPECT_EQ(x1, 0);
    EXPECT_EQ(y1, 0);
    EXPECT_EQ(x2, 5);
    EXPECT_EQ(y2, 5);
}

TEST(RectangleTest, Equality) {
    Rectangle r1(1, 1, 5, 5);
    Rectangle r2(1, 1, 5, 5);
    Rectangle r3(0, 0, 1, 1);

    EXPECT_TRUE(r1 == r2);
    EXPECT_FALSE(r1 == r3);
    EXPECT_TRUE(r1 != r3);
}

