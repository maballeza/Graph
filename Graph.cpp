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
    
    if (std::ofstream ofs{ output, std::ios::app }) {
        // Comparison of g before and after a call to Transpose()
        g.Breadth(1);
        g.Summarize(ofs);
        
        g.Transpose();
        
        g.Breadth(1);
        g.Summarize(ofs);
    }

    return g;
}

int main()
{
    std::vector<std::vector<const char*>> directed_input {
        { "a", "b", "c", "d", "e", "f" },
        { "b", "c", "f" },
        { "c", "b", "d" },
        { "d" },
        { "e", "d", "f" },
        { "f", "b", "e" }
    };

    auto g = DebugGraph<const char*>(directed_input, "Doc/Debug/Output_T.txt");
}