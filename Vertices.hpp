#pragma once
#include "List.hpp"
#include "Node.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <iomanip>

template <typename I>
struct Vertex : BaseNode<I> {
    enum Status { nf, f, d };   // Not found, found, discovered.

    Vertex(I&& i) :
        BaseNode<I>(std::forward<I>(i)),
        s{}, dist{}, t_found{}, t_disc{}, p{}, next{}, prev{}
    {
    }
    ~Vertex() = default;
    
    Status s;
    int dist;     // Distance        -> Graph::Breadth()
    int t_found;  // Time found      -> Graph::Depth()
    int t_disc;   // Time discovered -> Graph::Depth()
    Vertex* p; // Predecessor
    Vertex* next;
    Vertex* prev;
};

template <typename T>
using V = Vertex<T>;

template <typename I>
class Vertices {
public:
    using Vertex = Vertex<I>;
    using Edges = std::unordered_map<Vertex*, List<V, I>>;

    Vertices(int t) : 
        total{ t }, count{} 
    { 
        set[nullptr]; // Signals non-membership of a queried vertex.
    }

    List<V, I> BuildEdge(Vertex*, const std::vector<I>&);
    List<V, I>& operator[](Vertex*);

private:
    Edges set;
    int total;  // |set|
    int count;  // Running total.
};

template <typename I>
List<V, I>& Vertices<I>::operator[](Vertex* v)
{
    try {
        return set.at(v);
    }
    catch (std::out_of_range& /*re*/) {
        if (count < total) {
            return set[v];
        }
        else {
            return set.at(nullptr);
        }
    };
}

template <typename I>
List<V, I> Vertices<I>::BuildEdge(Vertex* v, const std::vector<I>& relations)
{
    List<V, I> l;
    for (I item : relations) {
        l.Insert(std::forward<I>(item));
    }
    ++count;
    return l;
}

template <typename I>
struct Summary {
    using Vertex = Vertex<I>;
    using string = std::string;

    Summary(std::vector<Vertex*>& vertices, std::ostream& os = std::cout) :
        vertices{ vertices }, os{ os },
        header{ "Vertex", "Distance", "Time Found", "Time Discovered", "Parent", "Status" },
        statuses{ "Not Found", "Found", "Discovered" }
    {
    }

    void Print() const { os << String(); }
    string String() const { return Header() + Body(); }

private:
    string SafeStringConversion(Vertex* v, char alternative = default_char) const;
    std::vector<string> Summarize(Vertex* v) const;
    string Header() const;
    string Body() const;

    std::ostream& os;
    std::vector<Vertex*> vertices;
    std::vector<string> header;
    std::vector<string> statuses;

    static inline const int col_width { 16 };
    static inline const char default_char { '-' };
};

template <typename I>
std::string Summary<I>::SafeStringConversion(Vertex* v, char alternative) const
{
    if constexpr (std::is_same_v<I, string> || std::is_convertible_v<I, string>) {
        return v->item;
    }
    return string{ alternative };
}

template <typename I>
std::vector<std::string> Summary<I>::Summarize(Vertex* v) const
{
    const int size = header.size();
    if (!v) {
        std::vector<string> empty(size, string{ default_char });
        return empty;
    }

    const auto name            = SafeStringConversion(v);
    const auto distance        = std::to_string(v->dist);
    const auto time_found      = std::to_string(v->t_found);
    const auto time_discovered = std::to_string(v->t_disc);
    const auto parent          = v->p ? SafeStringConversion(v->p) : string{ default_char };
    string status {};
    switch (v->s) {
        case Vertex::Status::nf : {
            status = "Not Found";
            break;
        }
        case Vertex::Status::f : {
            status = "Found";
            break;
        }
        case Vertex::Status::d : {
            status = "Discovered";
            break;
        }
    }

    return { name, distance, time_found, time_discovered, parent, status };
}

template <typename I>
std::string Summary<I>::Header() const
{
    std::ostringstream oss{};
    for (const auto& col_name : header) {
        oss << std::setw(col_width) << std::right << col_name;
    }
    return oss.str() + '\n';
}

template <typename I>
std::string Summary<I>::Body() const
{
    std::ostringstream oss{};
    for (const auto& n : vertices) {
        for (const auto& r : Summarize(n)) {
            oss << std::setw(col_width) << std::right << r;
        }
        oss << '\n';
    }
    return oss.str() + '\n';
}