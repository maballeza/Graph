#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "../Graph.hpp"

using ::testing::IsNull;
using ::testing::NotNull;
using ::testing::ElementsAre;
using ::testing::Pointee;
using ::testing::Eq;
using ::testing::Ne;

class GraphTest : public ::testing::Test {
protected:

    void SetUp() override
    {
        auto vs = directed.VertexSet();
        undirected.Breadth(vs[0]);      // Index
        directed.Breadth(vs[0]);
    }

    Graph<const char*> undirected {{ // a - b - c
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