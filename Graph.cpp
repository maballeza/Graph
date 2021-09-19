#include "Graph.hpp"
#include <vector>
#include <sstream>  // Debug
#include <fstream>  // Debug

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
Graph<I> DebugGraph(std::vector<std::vector<I>>& vertices, std::string& output = {})
{
    Graph<I> g { vertices };

    std::ostringstream oss{};
    for (int v_n{}; v_n < vertices.size(); ++v_n) {
        g.Breadth(v_n);
        g.Summarize(oss);
    }

    if (std::ofstream ofs { output }) {
        ofs << oss.str();
    }
    else {
        std::cout << oss.str();
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

    std::string output {
        "Doc\\Debug\\Output.txt"
    };
    auto g = DebugGraph<const char*>(undirected_input, output);
}