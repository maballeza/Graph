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
    int s = vertices.size();
    
    g.Breadth(0);
    for (auto& u : g.VertexSet()) {
        if (u->item == "a") {
            assert(g.InDegree(u) == 5);
            assert(g.OutDegree(u) == 5);
        }
        else {
            assert(g.InDegree(u) == 3);
            assert(g.OutDegree(u) == 3);
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

    auto g = DebugGraph<const char*>(undirected_input);
}