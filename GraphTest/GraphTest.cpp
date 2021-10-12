#include "GraphTest.hpp"
#include <string>

TEST_F(GraphTest, SetSize)
{
    ASSERT_EQ(this->undirected.VertexSet().size(), 3);
    ASSERT_EQ(this->directed.VertexSet().size(), 3);
}

TEST_F(GraphTest, SetOrder)
{
    std::vector<const char*> v_comp { "a", "b", "c" };
    std::vector<Vertex<const char*>*>& v = this->directed.VertexSet();

    ASSERT_EQ(v[0]->item, v_comp[0]);
    ASSERT_EQ(v[1]->item, v_comp[1]);
    ASSERT_EQ(v[2]->item, v_comp[2]);
}

TEST_F(GraphTest, MatchingEdgesUndirected)
{
    Graph<const char*>& g = this->undirected;

    for (Vertex<const char*>* v : g.VertexSet()) {
        auto edges = g.Edges(v);

        if (v->item == "a") {
            ASSERT_THAT(edges.Search("a"), IsNull());
            ASSERT_THAT(edges.Search("b"), NotNull());
            ASSERT_THAT(edges.Search("c"), IsNull());
        }
        else if (v->item == "b") {
            ASSERT_THAT(edges.Search("a"), NotNull());
            ASSERT_THAT(edges.Search("b"), IsNull());
            ASSERT_THAT(edges.Search("c"), NotNull());

        }
        else {
            ASSERT_THAT(edges.Search("a"), IsNull());
            ASSERT_THAT(edges.Search("b"), NotNull());
            ASSERT_THAT(edges.Search("c"), IsNull());
        }
    }
}

TEST_F(GraphTest, MatchingEdgesDirected)
{
    Graph<const char*>& g = this->directed;

    for (Vertex<const char*>* v : g.VertexSet()) {
        auto edges = g.Edges(v);

        if (v->item == "a") {
            ASSERT_THAT(edges.Search("a"), IsNull());
            ASSERT_THAT(edges.Search("b"), NotNull());
            ASSERT_THAT(edges.Search("c"), IsNull());
        }
        else if (v->item == "b") {
            ASSERT_THAT(edges.Search("a"), IsNull());
            ASSERT_THAT(edges.Search("b"), IsNull());
            ASSERT_THAT(edges.Search("c"), NotNull());

        }
        else {
            ASSERT_THAT(edges.Search("a"), IsNull());
            ASSERT_THAT(edges.Search("b"), IsNull());
            ASSERT_THAT(edges.Search("c"), IsNull());
        }
    }
}

TEST_F(GraphTest, InDegreeUndirected)
{
    Graph<const char*>& g = this->undirected;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];
    
    ASSERT_THAT(g.InDegree(a), Eq(1));
    ASSERT_THAT(g.InDegree(b), Eq(2));
    ASSERT_THAT(g.InDegree(c), Eq(1));
}

TEST_F(GraphTest, InDegreeDirected)
{
    Graph<const char*>& g = this->directed;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];
    
    ASSERT_THAT(g.InDegree(a), Eq(0));
    ASSERT_THAT(g.InDegree(b), Eq(1));
    ASSERT_THAT(g.InDegree(c), Eq(1));
}

TEST_F(GraphTest, OutDegreeUndirected)
{
    Graph<const char*>& g = this->undirected;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];

    ASSERT_THAT(g.OutDegree(a), Eq(1));
    ASSERT_THAT(g.OutDegree(b), Eq(2));
    ASSERT_THAT(g.OutDegree(c), Eq(1));
}

TEST_F(GraphTest, OutDegreeDirected)
{
    Graph<const char*>& g = this->directed;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];

    ASSERT_THAT(g.OutDegree(a), Eq(1));
    ASSERT_THAT(g.OutDegree(b), Eq(1));
    ASSERT_THAT(g.OutDegree(c), Eq(0));
}

TEST_F(GraphTest, BreadthUndirected)
{
    Graph<const char*>& g = this->undirected;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];
    
    auto discovered = Vertex<const char*>::Status::d;
    
    g.Breadth(a);
    ASSERT_EQ(a->s, discovered);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->dist, 0);
    ASSERT_EQ(b->dist, 1);
    ASSERT_EQ(c->dist, 2);
    ASSERT_THAT(a->p, IsNull());
    ASSERT_THAT(b->p, Eq(a));
    ASSERT_THAT(c->p, Eq(b));
        
    g.Breadth(b);
    ASSERT_EQ(a->s, discovered);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->dist, 1);
    ASSERT_EQ(b->dist, 0);
    ASSERT_EQ(c->dist, 1);
    ASSERT_THAT(a->p, Eq(b));
    ASSERT_THAT(b->p, IsNull());
    ASSERT_THAT(c->p, Eq(b));
        
    g.Breadth(c);
    ASSERT_EQ(a->s, discovered);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->dist, 2);
    ASSERT_EQ(b->dist, 1);
    ASSERT_EQ(c->dist, 0);
    ASSERT_THAT(a->p, Eq(b));
    ASSERT_THAT(b->p, Eq(c));
    ASSERT_THAT(c->p, IsNull());
}

TEST_F(GraphTest, BreadthDirected)
{
    Graph<const char*>& g = this->directed;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];
    
    auto discovered = Vertex<const char*>::Status::d;
    auto not_found = Vertex<const char*>::Status::nf;
    
    g.Breadth(a);
    ASSERT_EQ(a->s, discovered);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->dist, 0);
    ASSERT_EQ(b->dist, 1);
    ASSERT_EQ(c->dist, 2);
    ASSERT_THAT(a->p, IsNull());
    ASSERT_THAT(b->p, Eq(a));
    ASSERT_THAT(c->p, Eq(b));
    
    g.Breadth(b);
    ASSERT_EQ(a->s, not_found);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->dist, 100000);
    ASSERT_EQ(b->dist, 0);
    ASSERT_EQ(c->dist, 1);
    ASSERT_THAT(a->p, IsNull());
    ASSERT_THAT(b->p, IsNull());
    ASSERT_THAT(c->p, Eq(b));

    g.Breadth(c);
    ASSERT_EQ(a->s, not_found);
    ASSERT_EQ(b->s, not_found);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->dist, 100000);
    ASSERT_EQ(b->dist, 100000);
    ASSERT_EQ(c->dist, 0);
    ASSERT_THAT(a->p, IsNull());
    ASSERT_THAT(b->p, IsNull());
    ASSERT_THAT(c->p, IsNull());
}

TEST_F(GraphTest, DepthUndirected)
{
    Graph<const char*>& g = this->undirected;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];

    auto discovered = Vertex<const char*>::Status::d;
    auto not_found = Vertex<const char*>::Status::nf;
    
    g.Depth(a);
    ASSERT_EQ(a->s, discovered);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->t_found, 1);
    ASSERT_EQ(b->t_found, 2);
    ASSERT_EQ(c->t_found, 3);
    ASSERT_EQ(a->t_disc, 6);
    ASSERT_EQ(b->t_disc, 5);
    ASSERT_EQ(c->t_disc, 4);
    ASSERT_THAT(a->p, IsNull());
    ASSERT_THAT(b->p, Eq(a));
    ASSERT_THAT(c->p, Eq(b));
        
    g.Depth(b);
    ASSERT_EQ(a->s, discovered);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->t_found, 4);
    ASSERT_EQ(b->t_found, 1);
    ASSERT_EQ(c->t_found, 2);
    ASSERT_EQ(a->t_disc, 5);
    ASSERT_EQ(b->t_disc, 6);
    ASSERT_EQ(c->t_disc, 3);
    ASSERT_THAT(a->p, Eq(b));
    ASSERT_THAT(b->p, IsNull());
    ASSERT_THAT(c->p, Eq(b));
        
    g.Depth(c);
    ASSERT_EQ(a->s, discovered);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->t_found, 3);
    ASSERT_EQ(b->t_found, 2);
    ASSERT_EQ(c->t_found, 1);
    ASSERT_EQ(a->t_disc, 4);
    ASSERT_EQ(b->t_disc, 5);
    ASSERT_EQ(c->t_disc, 6);
    ASSERT_THAT(a->p, Eq(b));
    ASSERT_THAT(b->p, Eq(c));
    ASSERT_THAT(c->p, IsNull());
}

TEST_F(GraphTest, DepthDirected)
{
    Graph<const char*>& g = this->directed;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];

    auto discovered = Vertex<const char*>::Status::d;
    auto not_found = Vertex<const char*>::Status::nf;
    
    g.Depth(a);
    ASSERT_EQ(a->s, discovered);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->t_found, 1);
    ASSERT_EQ(b->t_found, 2);
    ASSERT_EQ(c->t_found, 3);
    ASSERT_EQ(a->t_disc, 6);
    ASSERT_EQ(b->t_disc, 5);
    ASSERT_EQ(c->t_disc, 4);
    ASSERT_THAT(a->p, IsNull());
    ASSERT_THAT(b->p, Eq(a));
    ASSERT_THAT(c->p, Eq(b));
    
    g.Depth(b);
    ASSERT_EQ(a->s, not_found);
    ASSERT_EQ(b->s, discovered);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->t_found, 0);
    ASSERT_EQ(b->t_found, 1);
    ASSERT_EQ(c->t_found, 2);
    ASSERT_EQ(a->t_disc, 0);
    ASSERT_EQ(b->t_disc, 4);
    ASSERT_EQ(c->t_disc, 3);
    ASSERT_THAT(a->p, IsNull());
    ASSERT_THAT(b->p, IsNull());
    ASSERT_THAT(c->p, Eq(b));
    
    g.Depth(c);
    ASSERT_EQ(a->s, not_found);
    ASSERT_EQ(b->s, not_found);
    ASSERT_EQ(c->s, discovered);
    ASSERT_EQ(a->t_found, 0);
    ASSERT_EQ(b->t_found, 0);
    ASSERT_EQ(c->t_found, 1);
    ASSERT_EQ(a->t_disc, 0);
    ASSERT_EQ(b->t_disc, 0);
    ASSERT_EQ(c->t_disc, 2);
    ASSERT_THAT(a->p, IsNull());
    ASSERT_THAT(b->p, IsNull());
    ASSERT_THAT(c->p, IsNull());
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
    
    g.Breadth(a);
    EXPECT_THAT(g.ShortestPath(a, b), ElementsAre(Pointee(v_a), Pointee(v_b)));
    EXPECT_THAT(g.ShortestPath(a, c), ElementsAre(Pointee(v_a), Pointee(v_b), Pointee(v_c)));
    EXPECT_THAT(g.ShortestPath(b, a), ElementsAre());
    EXPECT_THAT(g.ShortestPath(b, c), ElementsAre(Pointee(v_b), Pointee(v_c)));
    EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
    EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
    
    g.Breadth(b);
    EXPECT_THAT(g.ShortestPath(a, b), ElementsAre());
    EXPECT_THAT(g.ShortestPath(a, c), ElementsAre());
    EXPECT_THAT(g.ShortestPath(b, a), ElementsAre(Pointee(v_b), Pointee(v_a)));
    EXPECT_THAT(g.ShortestPath(b, c), ElementsAre(Pointee(v_b), Pointee(v_c)));
    EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
    EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
    
    g.Breadth(c);
    EXPECT_THAT(g.ShortestPath(a, b), ElementsAre());
    EXPECT_THAT(g.ShortestPath(a, c), ElementsAre());
    EXPECT_THAT(g.ShortestPath(b, a), ElementsAre(Pointee(v_b), Pointee(v_a)));
    EXPECT_THAT(g.ShortestPath(b, c), ElementsAre());
    EXPECT_THAT(g.ShortestPath(c, a), ElementsAre(Pointee(v_c), Pointee(v_b), Pointee(v_a)));
    EXPECT_THAT(g.ShortestPath(c, b), ElementsAre(Pointee(v_c), Pointee(v_b)));
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

    g.Breadth(a);
    EXPECT_THAT(g.ShortestPath(a, b), ElementsAre(Pointee(v_a), Pointee(v_b)));
    EXPECT_THAT(g.ShortestPath(a, c), ElementsAre(Pointee(v_a), Pointee(v_b), Pointee(v_c)));
    EXPECT_THAT(g.ShortestPath(b, a), ElementsAre());
    EXPECT_THAT(g.ShortestPath(b, c), ElementsAre(Pointee(v_b), Pointee(v_c)));
    EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
    EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
    
    g.Breadth(b);
    EXPECT_THAT(g.ShortestPath(a, b), ElementsAre());
    EXPECT_THAT(g.ShortestPath(a, c), ElementsAre());
    EXPECT_THAT(g.ShortestPath(b, a), ElementsAre());
    EXPECT_THAT(g.ShortestPath(b, c), ElementsAre(Pointee(v_b), Pointee(v_c)));
    EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
    EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
    
    g.Breadth(c);
    EXPECT_THAT(g.ShortestPath(a, b), ElementsAre());
    EXPECT_THAT(g.ShortestPath(a, c), ElementsAre());
    EXPECT_THAT(g.ShortestPath(b, a), ElementsAre());
    EXPECT_THAT(g.ShortestPath(b, c), ElementsAre());
    EXPECT_THAT(g.ShortestPath(c, a), ElementsAre());
    EXPECT_THAT(g.ShortestPath(c, b), ElementsAre());
}

TEST_F(GraphTest, TransposeUndirected)
{
    Graph<const char*>& g = this->undirected;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];
    Vertex<const char*> v_a { std::move("a") };
    Vertex<const char*> v_b { std::move("b") };
    Vertex<const char*> v_c { std::move("c") };

    g.Transpose();

    for (Vertex<const char*>* v : vs) {
        auto edges = g.Edges(v);
        if (v == a) {
            ASSERT_THAT(edges.Search("a"), IsNull());
            ASSERT_THAT(edges.Search("b"), Pointee(v_b));
            ASSERT_THAT(edges.Search("c"), IsNull());
        }
        else if (v == b) {
            ASSERT_THAT(edges.Search("a"), Pointee(v_a));
            ASSERT_THAT(edges.Search("b"), IsNull());
            ASSERT_THAT(edges.Search("c"), Pointee(v_c));
        }
        else {
            ASSERT_THAT(edges.Search("a"), IsNull());
            ASSERT_THAT(edges.Search("b"), Pointee(v_b));
            ASSERT_THAT(edges.Search("c"), IsNull());
        }
    }
}

TEST_F(GraphTest, TransposeDirected)
{
    Graph<const char*>& g = this->directed;
    auto vs = g.VertexSet();
    auto a = vs[0];
    auto b = vs[1];
    auto c = vs[2];
    Vertex<const char*> v_a { std::move("a") };
    Vertex<const char*> v_b { std::move("b") };
    Vertex<const char*> v_c { std::move("c") };

    g.Transpose();

    for (Vertex<const char*>* v : vs) {
        auto edges = g.Edges(v);
        if (v == a) {
            ASSERT_THAT(edges.Search("a"), IsNull());
            ASSERT_THAT(edges.Search("b"), IsNull());
            ASSERT_THAT(edges.Search("c"), IsNull());
        }
        else if (v == b) {
            ASSERT_THAT(edges.Search("a"), Pointee(v_a));
            ASSERT_THAT(edges.Search("b"), IsNull());
            ASSERT_THAT(edges.Search("c"), IsNull());
        }
        else {
            ASSERT_THAT(edges.Search("a"), IsNull());
            ASSERT_THAT(edges.Search("b"), Pointee(v_b));
            ASSERT_THAT(edges.Search("c"), IsNull());
        }
    }
}