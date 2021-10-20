#pragma once
#include "Vertices.hpp"
#include "GraphList.hpp"
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

    void RemoveVertex(Vertex*);
    void Breadth(Vertex*);
    void Depth(Vertex*);
    std::vector<Vertex*> ShortestPath(Vertex* s, Vertex* v);
    void Transpose();

    void Summarize(std::ostream& os);

    int InDegree(Vertex* v);
    int OutDegree(Vertex* v) { return vertices[v].Size(); }
    std::vector<Vertex*>& VertexSet() { return vertices.set; }
    GraphList<I>& Edges(Vertex* v) { return vertices[v]; }

private:
    Vertex* AcquireVertex(I&& list_head);
    void AddVertex(const std::vector<I>& list);
    void AddVertices(const std::vector<std::vector<I>>& lists);
    bool InGraph(Vertex*);
    static void Reset(Graph& g, Vertex* s = nullptr);
    static bool NotFound(const Vertex*);

    static void Breadth(Graph& g, Vertex* s);
    static void Depth(Graph& g, Vertex* s);
    static void Visit(Graph& g, Vertex* v);

    Vertices vertices;
    int time;
};

template <typename I>
Graph<I>::Graph(const std::vector<std::vector<I>>& incidentals_list) noexcept
    : vertices{ incidentals_list.size() }, time{}
{
    AddVertices(incidentals_list);
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
void Graph<I>::AddVertex(const std::vector<I>& incidentals)
{
    auto head = incidentals.begin();
    Vertex* v = AcquireVertex(std::forward<I>(I{ *head }));
    vertices.AttachVertex(v, { ++head, incidentals.end() });
}

template <typename I>
void Graph<I>::AddVertices(const std::vector<std::vector<I>>& incidentals_list)
{
    for (const std::vector<I>& incidentals : incidentals_list) {
        AddVertex(incidentals);
    }
}

template <typename I>
void Graph<I>::RemoveVertex(Vertex* v)
{
    vertices.RemoveVertex(v);
    Graph::Reset(*this);
}

template <typename I>
Vertex<I>* Graph<I>::AcquireVertex(I&& list_head)
{
    return Acquire<V, I>::Instance(std::forward<I>(list_head)).Release();
}

template <typename I>
bool Graph<I>::InGraph(Vertex* w)
{
    return vertices.InGraph(w);
}

template <typename I>
void Graph<I>::Reset(Graph& g, Vertex* source)
{
    for (Vertex* v : g.vertices) {
        Vertex::Reset(v, v == source);
    }
    g.time = 0;
}

template <typename I>
bool Graph<I>::NotFound(const Vertex* v) {
    return v->s == Vertex::Status::nf;
}

template <typename I>
void Graph<I>::Breadth(Vertex* v)
{
    if(InGraph(v)) {
        Graph::Breadth(*this, v);
    }
}

template <typename I>
void Graph<I>::Depth(Vertex* v)
{
    if (InGraph(v)) {
        Graph::Depth(*this, v);
    }
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
void Graph<I>::Depth(Graph& g, Vertex* source)
{
    Graph::Reset(g);
    bool source_found{};
    auto stand_in = g.VertexSet();
    stand_in.reserve(stand_in.size() * 2);
    for (Vertex* v : stand_in) {
        if (!source_found && v != source) {
            stand_in.push_back(v);
            continue;
        }
        else if (NotFound(v)) {
            source_found = true;
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
    if (s && v) {
        vertices.ShortestPath(s, v, path);
    }
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