#pragma once
#include "Node.hpp"

template <typename I>
struct GraphNode : BaseNode<I> {
    enum Status { nf, f, d };   // Not found, found, discovered.

    GraphNode(I&& i) :
        BaseNode<I>(std::forward<I>(i)),
        s{}, dist{}, t_found{}, t_disc{}, p{}, next{}, prev{}
    {}
    ~GraphNode() {};
    
    Status s;
    
    int dist;    // Breadth
    int t_found; // Depth: Time found
    int t_disc;  // Depth: Time discovered
    
    GraphNode* p;     // Predecessor
    GraphNode* next;
    GraphNode* prev;
};