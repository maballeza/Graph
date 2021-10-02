#include "GraphTest.hpp"
#include <string>

TEST_F(GraphTest, SetSize)
{
    ASSERT_EQ(this->undirected.VertexSet().size(), 3);
    ASSERT_EQ(this->directed.VertexSet().size(), 3);
}

TEST_F(GraphTest, SetOrder)
{
    std::vector<const char*> v { "a", "b", "c" };

    for (int i{}; i < v.size(); ++i) {
        ASSERT_EQ(this->undirected.VertexSet()[i]->item, v[i]);
        ASSERT_EQ(this->directed.VertexSet()[i]->item, v[i]);
    }
}

TEST_F(GraphTest, MatchingEdgesUndirected)
{
    Graph<const char*>& g = this->undirected;

    for (Vertex<const char*>* v : g.VertexSet()) {
        auto edges = g.Edges(v);

        if (v->item == "a") {
            ASSERT_EQ(edges.Search("a"), nullptr);
            ASSERT_NE(edges.Search("b"), nullptr);
            ASSERT_EQ(edges.Search("c"), nullptr);
        }
        else if (v->item == "b") {
            ASSERT_NE(edges.Search("a"), nullptr);
            ASSERT_EQ(edges.Search("b"), nullptr);
            ASSERT_NE(edges.Search("c"), nullptr);

        }
        else {
            ASSERT_EQ(edges.Search("a"), nullptr);
            ASSERT_NE(edges.Search("b"), nullptr);
            ASSERT_EQ(edges.Search("c"), nullptr);
        }
    }
}

TEST_F(GraphTest, MatchingEdgesDirected)
{
    Graph<const char*>& g = this->directed;

    for (Vertex<const char*>* v : g.VertexSet()) {
        auto edges = g.Edges(v);

        if (v->item == "a") {
            ASSERT_EQ(edges.Search("a"), nullptr);
            ASSERT_NE(edges.Search("b"), nullptr);
            ASSERT_EQ(edges.Search("c"), nullptr);
        }
        else if (v->item == "b") {
            ASSERT_EQ(edges.Search("a"), nullptr);
            ASSERT_EQ(edges.Search("b"), nullptr);
            ASSERT_NE(edges.Search("c"), nullptr);

        }
        else {
            ASSERT_EQ(edges.Search("a"), nullptr);
            ASSERT_EQ(edges.Search("b"), nullptr);
            ASSERT_EQ(edges.Search("c"), nullptr);
        }
    }
}

TEST_F(GraphTest, InDegreeUndirected)
{
    Graph<const char*>& g = this->undirected;
    auto set = g.VertexSet();

    for (Vertex<const char*>* v : set) {
        if (v->item != "b") {
            ASSERT_EQ(g.InDegree(v), 1);
        }
        else {
            ASSERT_EQ(g.InDegree(v), 2);
        }
    }
}

TEST_F(GraphTest, InDegreeDirected)
{
    Graph<const char*>& g = this->directed;
    auto set = g.VertexSet();
    
    for (Vertex<const char*>* v : set) {
        if (v->item != "a") {
            ASSERT_EQ(g.InDegree(v), 1);
        }
        else {
            ASSERT_EQ(g.InDegree(v), 0);
        }
    }
}

TEST_F(GraphTest, OutDegreeUndirected)
{
    Graph<const char*>& g = this->undirected;
    auto set = g.VertexSet();
    
    for (Vertex<const char*>* v : set) {
        if (v->item != "b") {
            ASSERT_EQ(g.OutDegree(v), 1);
        }
        else {
            ASSERT_EQ(g.OutDegree(v), 2);
        }
    }
}

TEST_F(GraphTest, OutDegreeDirected)
{
    Graph<const char*>& g = this->directed;
    auto set = g.VertexSet();

    for (Vertex<const char*>* v : set) {
        if (v->item != "c") {
            ASSERT_EQ(g.OutDegree(v), 1);
        }
        else {
            ASSERT_EQ(g.OutDegree(v), 0);
        }
    }
}

TEST_F(GraphTest, ShortestPathUndirected)
{
    Graph<const char*>& g = this->undirected;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];
    Vertex<const char*> v_a { std::move("a") };
    Vertex<const char*> v_b { std::move("b") };
    Vertex<const char*> v_c { std::move("c") };
    
    for (Vertex<const char*>* v : vs) {
        g.Breadth(v);
        if (v == a) {
            EXPECT_THAT(g.ShortestPath(a, b), ElementsAre(Pointee(v_a), Pointee(v_b)));
            EXPECT_THAT(g.ShortestPath(a, c), ElementsAre(Pointee(v_a), Pointee(v_b), Pointee(v_c)));
            EXPECT_THAT(g.ShortestPath(b, a), ElementsAre());
            EXPECT_THAT(g.ShortestPath(b, c), ElementsAre(Pointee(v_b), Pointee(v_c)));
            EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
            EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
        }
        else if (v == b) {
            EXPECT_THAT(g.ShortestPath(a, b), ElementsAre());
            EXPECT_THAT(g.ShortestPath(a, c), ElementsAre());
            EXPECT_THAT(g.ShortestPath(b, a), ElementsAre(Pointee(v_b), Pointee(v_a)));
            EXPECT_THAT(g.ShortestPath(b, c), ElementsAre(Pointee(v_b), Pointee(v_c)));
            EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
            EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
        }
        else {
            EXPECT_THAT(g.ShortestPath(a, b), ElementsAre());
            EXPECT_THAT(g.ShortestPath(a, c), ElementsAre());
            EXPECT_THAT(g.ShortestPath(b, a), ElementsAre(Pointee(v_b), Pointee(v_a)));
            EXPECT_THAT(g.ShortestPath(b, c), ElementsAre());
            EXPECT_THAT(g.ShortestPath(c, a), ElementsAre(Pointee(v_c), Pointee(v_b), Pointee(v_a)));
            EXPECT_THAT(g.ShortestPath(c, b), ElementsAre(Pointee(v_c), Pointee(v_b)));
        }
    }
}

TEST_F(GraphTest, ShortestPathDirected)
{
    Graph<const char*>& g = this->directed;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];
    Vertex<const char*> v_a { std::move("a") };
    Vertex<const char*> v_b { std::move("b") };
    Vertex<const char*> v_c { std::move("c") };

    for (Vertex<const char*>* v : vs) {
        g.Breadth(v);
        if (v == a) {
            EXPECT_THAT(g.ShortestPath(a, b), ElementsAre(Pointee(v_a), Pointee(v_b)));
            EXPECT_THAT(g.ShortestPath(a, c), ElementsAre(Pointee(v_a), Pointee(v_b), Pointee(v_c)));
            EXPECT_THAT(g.ShortestPath(b, a), ElementsAre());
            EXPECT_THAT(g.ShortestPath(b, c), ElementsAre(Pointee(v_b), Pointee(v_c)));
            EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
            EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
        }
        else if (v == b) {
            EXPECT_THAT(g.ShortestPath(a, b), ElementsAre());
            EXPECT_THAT(g.ShortestPath(a, c), ElementsAre());
            EXPECT_THAT(g.ShortestPath(b, a), ElementsAre());
            EXPECT_THAT(g.ShortestPath(b, c), ElementsAre(Pointee(v_b), Pointee(v_c)));
            EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
            EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
        }
        else {
            EXPECT_THAT(g.ShortestPath(a, b), ElementsAre());
            EXPECT_THAT(g.ShortestPath(a, c), ElementsAre());
            EXPECT_THAT(g.ShortestPath(b, a), ElementsAre());
            EXPECT_THAT(g.ShortestPath(b, c), ElementsAre());
            EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
            EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
        }
    }
}

TEST_F(GraphTest, Transpose) {}