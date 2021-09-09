#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <type_traits>
#include "GraphNode.hpp"


template <typename I>
struct Summary {
    using Nodes = std::vector<GraphNode<I>*>;
    using string = std::string;

    Summary(Nodes& nodes, std::ostream& os = std::cout) :
        os { os },
        nodes { nodes },
        header { "Node", "Distance", "Time Found", "Time Discovered", "Parent", "Status" },
        statuses { "Not Found", "Found", "Discovered" }
    {}

    void Print() const {
        os << String();
    }

    string String() const {
        return Header() + Body();
    }

private:
    string SafeStringConversion(GraphNode<I>* node, char alternative = default_char) const;
    std::vector<string> Node(GraphNode<I>* node) const;
    string Header() const;
    string Body() const;

    std::ostream& os;

    Nodes nodes;
    static const int col_width;
    static const char default_char;
    std::vector<string> header;
    std::vector<string> statuses;
};

template <typename I> const int Summary<I>::col_width { 16 };
template <typename I> const char Summary<I>::default_char { '-' };

template <typename I>
std::string Summary<I>::SafeStringConversion(GraphNode<I>* node, char alternative) const {
    if constexpr (std::is_same_v<I, string> || std::is_convertible_v<I, string>) {
        return node->item;
    }
    return string{ alternative };
}

template <typename I>
std::vector<std::string> Summary<I>::Node(GraphNode<I>* node) const {
    const int size = header.size();
    if (!node) {
        std::vector<string> empty(size, string{ default_char });
        return empty;
    }

    const auto name = SafeStringConversion(node);
    const auto distance = std::to_string(node->dist);
    const auto time_found = std::to_string(node->t_found);
    const auto time_discovered = std::to_string(node->t_disc);
    const auto parent = node->p ? SafeStringConversion(node->p) : string{ default_char };
    const auto status = statuses[node->s];

    return { name, distance, time_found, time_discovered, parent, status };
}

template <typename I>
std::string Summary<I>::Header() const {
    std::ostringstream oss{};
    for (const auto& col_name : header) {
        oss << std::setw(col_width) << std::right << col_name;
    }
    return oss.str() + '\n';
}

template <typename I>
std::string Summary<I>::Body() const {
    std::ostringstream oss{};
    for (const auto& n : nodes) {
        for (const auto& r : Node(n)) {
            oss << std::setw(col_width) << std::right << r;
        }
        oss << '\n';
    }
    return oss.str() + '\n';
}