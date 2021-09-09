#pragma once
#include <vector>
#include <unordered_map>

#include "GraphNode.hpp"
#include "Representation.hpp"
#include "List.hpp"
#include "Queue.hpp"

template <typename I>
class Graph {
public:
    using Node = GraphNode<I>;

    Graph(std::vector<std::vector<I>>& lists) noexcept;
    Graph(Graph&&) = default;
    ~Graph();

    void Breadth();
    void Depth();
    void ShortestPath();
    void Transpose();

    void Summary();

private:
    /**
    * @param relations
    *   A set of lists, each containing, in turn, a unique vertex at the head followed by its immediate descendants.
    */
    List<GraphNode, I> BuildRelations(std::vector<I>&& relations);
    Node* BuildNode(I&& list_head);

    void Breadth(Graph& g, Node* s);
    void Depth(Graph& g);
    void DVisit(Graph& g, Node* n);
    void ShortestPath(Node* s, Node* v);
    void Transpose(Graph& g);

    int time;
    std::vector<Node*> set;
    std::unordered_map<Node*, List<GraphNode, I>> V;
};

template <typename I>
Graph<I>::Graph(std::vector<std::vector<I>>& lists) noexcept {
    for (std::vector<I>& list : lists) {
        auto head = list.begin();
        Node* node = BuildNode(std::move(*(head)));
        V[node] = BuildRelations({ ++head, list.end() });
        set.push_back(node);
    }
}

template <typename I>
Graph<I>::~Graph() {
    for (Node* n : set) {
        HNode<GraphNode, I>{ n };  // Deallocates upon destruction.
        n = nullptr;
    }
}

template <typename I>
typename Graph<I>::Node* Graph<I>::BuildNode(I&& list_head) {
    return Build<GraphNode, I>::Instance(std::forward<I>(list_head)).Release();
}

template <typename I>
List<GraphNode, I> Graph<I>::BuildRelations(std::vector<I>&& relations) {
    List<GraphNode, I> l;
    for (I item : relations) {
        l.Insert(std::move(item));
    }
    return l;
}

template <typename I>
void Graph<I>::Breadth() {
    Graph::Breadth(*this, set.front());
}

template <typename I>
void Graph<I>::Depth() {
    Graph::Depth(*this);
}

template <typename I>
void Graph<I>::ShortestPath() {
    Graph::ShortestPath(set.front(), set.back());
}

template <typename I>
void Graph<I>::Transpose() {
    Graph::Transpose(*this);
}

template <typename I>
void Graph<I>::Breadth(Graph& g, Node* s) {
    for (Node* k : g.set) {
        k->dist = 1000000;
        k->s = Node::Status::nf;
        k->p = nullptr;
    }
    s->dist = 0;
    Queue<GraphNode, I> Q;
    Q.Enqueue(s);
    while (Node* u = Q.Dequeue()) {
        u->s = Node::Status::f;
        for (Node* v : g.V[u]) {
            if (v->s == Node::Status::nf) {
                v->p = u;
                v->dist = u->dist + 1;
                Q.Enqueue(v);
            }
        }
        u->s = Node::Status::d;
    }
}

template <typename I>
void Graph<I>::Depth(Graph& g) {
    for (Node* n : g.set) {
        n->t_disc = 0;
        n->t_found = 0;
        n->s = Node::Status::nf;
        n->p = nullptr;
    }
    for (Node* n : g.set) {
        if (n->s == Node::Status::nf) {
            DVisit(g, n);
        }
    }
}

template <typename I>
void Graph<I>::DVisit(Graph& g, Node* n) {
    n->t_found = ++g.time;
    n->s = Node::Status::f;
    for (Node* u : g.V[n]) {
        if (u->s == Node::Status::nf) {
            u->p = n;
            DVisit(g, u);
        }
    }
    n->t_disc = ++g.time;
    n->s = Node::Status::d;
}

template <typename I>
void Graph<I>::ShortestPath(Node* s, Node* v) {}

template <typename I>
void Graph<I>::Transpose(Graph& g) {}
    

template <typename I>
void Graph<I>::Summary() {
    Representation<I>{ set }.Print();
}