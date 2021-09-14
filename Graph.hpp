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
    Graph(Graph&&) = default;
    ~Graph();

    void Breadth(int v);
    void Depth();
    void ShortestPath();
    void Transpose();

    void Summarize(std::ostream& os);

private:
    Vertex* AcquireVertex(I&& list_head);
    static Vertex* Normalize(Graph& g, Vertex* list_v);

    static void Breadth(Graph& g, Vertex* s);
    static void Depth(Graph& g);
    static void DVisit(Graph& g, Vertex* v);
    static void ShortestPath(Vertex* s, Vertex* v);
    static void Transpose(Graph& g);

    int time;
    std::vector<Vertex*> set;
    Vertices vertices;
};

template <typename I>
Graph<I>::Graph(std::vector<std::vector<I>>& lists) noexcept :
    vertices{ lists.size() }
{
    for (std::vector<I>& list : lists) {
        auto head = list.begin();
        Vertex* v = AcquireVertex(std::forward<I>(*(head)));
        vertices[v] = vertices.BuildEdge(v, {++head, list.end()});
        set.push_back(v);
    }
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
Vertex<I>* Graph<I>::Normalize(Graph& g, Vertex* list_v)
{
    //return g.V.map[list_v];
    for (auto& v : g.set) {
        if (v->item == list_v->item) {
            return v;
        }
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
    for (Vertex* k : g.set) {
        k->dist = 1000000;
        k->s = Vertex::Status::nf;
        k->p = nullptr;
    }
    source->dist = 0;
    Queue<V, I> Q;
    Q.Enqueue(source);
    while (Vertex* u = Q.Dequeue()) {
        u->s = Vertex::Status::f;
        for (Vertex* v : g.vertices[u]) {
            Vertex* w = Normalize(g, v);
            if (w->s == Vertex::Status::nf) {
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
    for (Vertex* v : g.set) {
        v->t_disc = 0;
        v->t_found = 0;
        v->s = Vertex::Status::nf;
        v->p = nullptr;
    }
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
    Summary<I>{ set, os }.Print();
}