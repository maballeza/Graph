#pragma once
#include "Vertices.hpp"
#include "List.hpp"
#include "Queue.hpp"
#include <vector>
#include <iostream> // Debug

template <typename T>
using V = Vertex<T>;

template <typename I>
class Graph {
public:
    using Vertex = Vertex<I>;
    using Vertices = Vertices<I>;

    /**
    * @param lists
    *   A set of lists, each containing a set of vertices signaling the direct descendancy of each one to the first.
    */
    Graph(std::vector<std::vector<I>>& lists) noexcept;
    Graph(Graph&&) noexcept;
    ~Graph();

    void Breadth(int v);
    void Depth();
    void ShortestPath();
    void Transpose();

    void Summarize(std::ostream& os);

private:
    Vertex* AcquireVertex(I&& list_head);
    static Vertex* Normalize(const Graph& g, const Vertex* list_v);
    static void Reset(Graph& g, Vertex* s = nullptr);

    static void Breadth(Graph& g, Vertex* s);
    static void Depth(Graph& g);
    static void DVisit(Graph& g, Vertex* v);
    static void ShortestPath(Vertex* s, Vertex* v);
    static void Transpose(Graph& g);

    Vertices vertices;
    std::vector<Vertex*> set;
    int time;
};

template <typename I>
Graph<I>::Graph(std::vector<std::vector<I>>& lists) noexcept
    : vertices{ lists.size() }, set{}, time{}
{
    for (std::vector<I>& list : lists) {
        auto head = list.begin();
        Vertex* v = AcquireVertex(std::forward<I>(*head));
        vertices[v] = vertices.BuildEdge({ ++head, list.end() });
        set.push_back(v);
    }
}

template <typename I>
Graph<I>::Graph(Graph&& g) noexcept 
    : vertices{ std::move(g.vertices) }, set{ std::move(g.set) }, time{ g.time }
{
    g.time = 0;
}

template <typename I>
Graph<I>::~Graph()
{
    for (Vertex* v : set) {
        delete v;
        v = nullptr;
    }
}

template <typename I>
Vertex<I>* Graph<I>::AcquireVertex(I&& list_head)
{
    return Acquire<V, I>::Instance(std::forward<I>(list_head)).Release();
}

template <typename I>
Vertex<I>* Graph<I>::Normalize(const Graph& g, const Vertex* list_v)
{
    for (auto& v : g.set) {
        if (v->item == list_v->item) {
            return v;
        }
    }
}

template <typename I>
void Graph<I>::Reset(Graph& g, Vertex* source)
{
    for (Vertex* v : g.set) {
        if (v != source) {
            v->dist = 100000;
            v->s = Vertex::Status::nf;
            v->t_disc = 0;
            v->t_found = 0;
            v->p = nullptr;
        }
    }
    if (source) {
        source->dist = 0;
        source->p = nullptr;
    }
}

template <typename I>
void Graph<I>::Breadth(int vertex)
{
    const size_t s = set.size();
    if (0 <= vertex && vertex <= s - 1) {
        Graph::Breadth(*this, set[vertex]);
    }
}

template <typename I>
void Graph<I>::Depth()
{
    Graph::Depth(*this);
}

template <typename I>
void Graph<I>::ShortestPath()
{
    Graph::ShortestPath(set.front(), set.back());
}

template <typename I>
void Graph<I>::Transpose()
{
    Graph::Transpose(*this);
}

template <typename I>
void Graph<I>::Breadth(Graph& g, Vertex* source)
{
    Graph::Reset(g, source);
    Queue<V, I> Q;
    Q.Enqueue(source);
    while (Vertex* u = Q.Dequeue()) {
        u->s = Vertex::Status::f;
        for (auto v : g.vertices[u]) {
            Vertex* w = Normalize(g, v);
            if (w->s == Vertex::Status::nf) {
                w->s = Vertex::Status::f;
                w->p = u;
                w->dist = u->dist + 1;
                Q.Enqueue(w);
            }
        }
        u->s = Vertex::Status::d;
    }
}

template <typename I>
void Graph<I>::Depth(Graph& g)
{
    Graph::Reset(g);
    for (Vertex* v : g.set) {
        if (v->s == Vertex::Status::nf) {
            DVisit(g, v);
        }
    }
}

template <typename I>
void Graph<I>::DVisit(Graph& g, Vertex* v)
{
    v->t_found = ++g.time;
    v->s = Vertex::Status::f;
    for (Vertex* u : g.vertices[v]) {
        if (u->s == Vertex::Status::nf) {
            u->p = v;
            DVisit(g, u);
        }
    }
    v->t_disc = ++g.time;
    v->s = Vertex::Status::d;
}

template <typename I>
void Graph<I>::ShortestPath(Vertex* s, Vertex* v)
{}

template <typename I>
void Graph<I>::Transpose(Graph& g)
{}

template <typename I>
void Graph<I>::Summarize(std::ostream& os)
{
    Summary<I> { set, os }.Print();
}