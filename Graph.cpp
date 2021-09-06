#include <vector>
#include "Graph.hpp"

template <typename I>
using Node = GraphNode<I>;

template <typename I>
HNode<Node, I> TestNode() {
    List<Node, I> list {};
    I item = I{};
    HNode<Node, I> n = Build<Node, I>::Instance(std::move(item));
    list.Insert(n.Release());
    auto new_list { std::move(list) };
    return new_list.Release(I{});
}

template <typename I>
Graph<I> DebugGraph(std::vector<std::vector<I>>& strings) {
    Graph<I> g { strings };

    g.Breadth();
    g.Depth();
    g.ShortestPath();

    return g;
}

int main() {
    std::vector<std::vector<const char*>> strings {
        { "a", "b", "c" , "d" , "e" , "f" },
        { "b", "c", "f" },
        { "c", "d", "b" },
        { "d", "e", "c" },
        { "e", "f", "d" },
        { "f", "b", "e" },
    };

    auto n = TestNode<int>();
    auto g = DebugGraph<const char*>(strings);
}