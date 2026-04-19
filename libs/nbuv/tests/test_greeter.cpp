#include <gtest/gtest.h>

#include "nbuv/greeter.hpp"

TEST(Greeter, DefaultName) {
    nbuv::Greeter g;
    EXPECT_EQ(g.name(), "world");
    EXPECT_EQ(g.greet(), "Hello, world!");
}

TEST(Greeter, CustomNameAndRename) {
    nbuv::Greeter g{"nanobind"};
    EXPECT_EQ(g.greet(), "Hello, nanobind!");

    g.set_name("uv");
    EXPECT_EQ(g.name(), "uv");
    EXPECT_EQ(g.greet(), "Hello, uv!");
}
