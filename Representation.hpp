#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include "GraphNode.hpp"

template <typename I>
struct Representation {
    using Nodes = std::vector<GraphNode<I>*>;

    Representation(Nodes& nodes, std::ostream& os = std::cout) : 
        nodes { nodes }, 
        header { "Node", "Distance", "Time Found", "Time Discovered", "Parent", "Color" },
        col_width { 16 },
        default_char { '-' },
        os { os },
        statuses { "Not Found", "Found", "Discovered" }
    {}

    void Print() const {
        os << String();
    }

    std::string String() const {
        return Header() + Body();
        //std::string representation { Header() + Body() };
        //return representation;
    }

private:
    std::vector<std::string> NodeRepresentation(GraphNode<I>* node) const;

    std::string Header() const {
        std::ostringstream oss{};
        for (const auto& col_name : header) {
            oss << std::setw(col_width) << std::right << col_name;
        }
        return oss.str() + '\n';
    }

    std::string Body() const {
        std::ostringstream oss{};
        for (const auto& n : nodes) {
            for (const auto& r : NodeRepresentation(n)) {
                oss << std::setw(col_width) << std::right << r;
            }
            oss << '\n';
        }
        return oss.str() + '\n';
    }

    std::ostream& os;

    Nodes nodes;
    std::vector<std::string> header;
    const int col_width;
    const char default_char;
    std::vector<std::string> statuses;
};

template <typename I>
std::vector<std::string> Representation<I>::NodeRepresentation(GraphNode<I>* node) const {
    const int size = header.size();
    if (!node) {
        std::vector<std::string> empty(size, std::to_string(default_char));
        return empty;
    }
    //!
    const auto distance = std::to_string(node->dist);
    const auto time_found = std::to_string(node->t_found);
    const auto time_discovered = std::to_string(node->t_disc);
    //!
    const std::string name = "name";
    const std::string parent = "parent";
    const std::string color = statuses[node->s];
    return { name, distance, time_found, time_discovered, parent, color };
}