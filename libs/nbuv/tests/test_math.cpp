#include <gtest/gtest.h>

#include "nbuv/math.hpp"

TEST(Math, AddReturnsSum) {
    EXPECT_EQ(nbuv::add(1, 2), 3);
    EXPECT_EQ(nbuv::add(-5, 5), 0);
}

TEST(Math, SumOfSequence) {
    EXPECT_DOUBLE_EQ(nbuv::sum({1.0, 2.0, 3.5}), 6.5);
    EXPECT_DOUBLE_EQ(nbuv::sum({}), 0.0);
}
