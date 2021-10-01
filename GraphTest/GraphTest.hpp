#pragma once
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include "../Graph.hpp"

using ::testing::ElementsAre;
using ::testing::ContainerEq;
using ::testing::IsEmpty;

class GraphTest : public ::testing::Test {
protected:

    void SetUp() override
    {
        undirected.Breadth(0);      // Index
        directed.Breadth(0);
    }

    Graph<const char*> undirected{{ // a - b - c
        { "a", "b" },
        { "b", "a", "c" },
        { "c", "b" }
    }};

    Graph<const char*> directed {{  // a -> b -> c
        { "a", "b" },
        { "b", "c" },
        { "c" }
    }};
};