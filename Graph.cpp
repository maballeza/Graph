#include "Graph.hpp"
#include <vector>
#include <sstream>  // Debug
#include <fstream>  // Debug
#include <assert.h>

template <typename I>
HNode<V, I> TestVertex()
{
    List<V, I> list{};
    I item = I{};
    HNode<V, I> n = Acquire<V, I>::Instance(std::move(item));
    list.Insert(n.Release());
    auto new_list { std::move(list) };
    return new_list.Release(I{});
}

template <typename I>
Graph<I> DebugGraph(std::vector<std::vector<I>>& vertices, const std::string& output = {})
{
    Graph<I> g { vertices };
    
    g.Breadth(0);
    
    auto vs = g.VertexSet();
    auto u = vs[0]; // "a"
    for (auto v : vs) {
        if (v != u) {
            assert(v->IncidentFrom(u));
            assert(u->IncidentTo(v));
        }
    }

    return g;
}

int main()
{
    std::vector<std::vector<const char*>> undirected_input {
        { "a", "b", "c", "d", "e", "f" },
        { "b", "c", "f", "a" },
        { "c", "d", "b", "a" },
        { "d", "e", "c", "a" },
        { "e", "f", "d", "a" },
        { "f", "b", "e", "a" }
    };

    auto g = DebugGraph<const char*>(undirected_input, "Doc/Debug/Output_SP.txt");
}