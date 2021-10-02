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
    Graph(const std::vector<std::vector<I>>& lists) noexcept;
    Graph(Graph&&) noexcept;
    ~Graph();

    void Breadth(int v);
    void Depth();
    void Transpose();
    std::vector<Vertex*> ShortestPath(Vertex* s, Vertex* v);

    void Summarize(std::ostream& os);
    std::vector<Vertex*>& VertexSet() { return vertices.set; }
    List<V, I>& Edges(Vertex* v) { return vertices[v]; }

    int InDegree(Vertex* v);
    int OutDegree(Vertex* v) { return vertices[v].Size(); }

private:
    Vertex* AcquireVertex(I&& list_head);
    void Normalize(Vertex** list_v);
    static void Reset(Graph& g, Vertex* s = nullptr);
    static bool NotFound(const Vertex*);

    static void Breadth(Graph& g, Vertex* s);
    static void Depth(Graph& g);
    static void Visit(Graph& g, Vertex* v);

    Vertices vertices;
    int time;
};

template <typename I>
Graph<I>::Graph(const std::vector<std::vector<I>>& incidentals) noexcept
    : vertices{ incidentals.size() }, time{}
{
    for (const std::vector<I>& list : incidentals) {
        auto head = list.begin();
        Vertex* v = AcquireVertex(std::forward<I>(I{ *head }));
        vertices.AttachVertex(v, { ++head, list.end() });
    }
}

template <typename I>
Graph<I>::Graph(Graph&& g) noexcept 
    : vertices{ std::move(g.vertices) }, time{ g.time }
{
    g.time = 0;
}

template <typename I>
Graph<I>::~Graph()
{
    for (Vertex* v : vertices.set) {
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
void Graph<I>::Normalize(Vertex** list_v)
{
    vertices.Normalize(list_v);
}

template <typename I>
void Graph<I>::Reset(Graph& g, Vertex* source)
{
    for (Vertex* v : g.vertices) {
        if (v != source) {
            v->dist = 100000;
            v->s = Vertex::Status::nf;
            v->t_disc = 0;
            v->t_found = 0;
            v->p = nullptr;
        }
    }
    if (source) {
        source->s = Vertex::Status::f;
        source->dist = 0;
        source->p = nullptr;
    }
    g.time = 0;
}

template <typename I>
bool Graph<I>::NotFound(const Vertex* v) {
    return v->s == Vertex::Status::nf;
}

template <typename I>
void Graph<I>::Breadth(int vertex)
{
    const int s = vertices.Size();
    if (0 <= vertex && vertex <= s - 1) {
        Graph::Breadth(*this, vertices.set[vertex]);
    }
}

template <typename I>
void Graph<I>::Depth()
{
    Graph::Depth(*this);
}

template <typename I>
void Graph<I>::Transpose()
{
    vertices.Transpose();
}

template <typename I>
void Graph<I>::Breadth(Graph& g, Vertex* source)
{
    Graph::Reset(g, source);
    Queue<V, I> Q;
    Q.Enqueue(source);
    while (Vertex* u = Q.Dequeue()) {
        for (auto v : g.vertices[u]) {
            g.Normalize(&v);
            if (NotFound(v)) {
                v->p = u;
                v->dist = u->dist + 1;
                v->s = Vertex::Status::f;
                Q.Enqueue(v);
            }
        }
        u->s = Vertex::Status::d;
    }
}

template <typename I>
void Graph<I>::Depth(Graph& g)
{
    Graph::Reset(g);
    for (Vertex* v : g.vertices) {
        if (NotFound(v)) {
            Visit(g, v);
        }
    }
}

template <typename I>
void Graph<I>::Visit(Graph& g, Vertex* v)
{
    v->t_found = ++g.time;
    v->s = Vertex::Status::f;
    for (Vertex* u : g.vertices[v]) {
        g.Normalize(&u);
        if (NotFound(u)) {
            u->p = v;
            Visit(g, u);
        }
    }
    v->t_disc = ++g.time;
    v->s = Vertex::Status::d;
}

template <typename I>
std::vector<Vertex<I>*> Graph<I>::ShortestPath(Vertex* s, Vertex* v)
{
    std::vector<Vertex*> path;
    vertices.ShortestPath(s, v, path);
    return path;
}

template <typename I>
void Graph<I>::Summarize(std::ostream& os)
{
    Summary<I>{ vertices.set, os }.Print();
}

template <typename I>
int Graph<I>::InDegree(Vertex* v)
{
    int degree{};
    for (auto u : vertices.set) {
        for (auto incidental : vertices[u]) {
            if (incidental->item == v->item) {
                ++degree;
            }
        }
    }
    return degree;
}