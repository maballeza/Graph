#pragma once
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

    Vertex(I&& i)
        : BaseNode<I>(std::forward<I>(i)),
          s{}, dist{}, t_found{}, t_disc{}, p{}, next{}, prev{}
    {
    }
    ~Vertex() = default;
    bool operator==(const Vertex& v) const { return this->item == v.item; }
    bool operator!=(const Vertex& v) const { return !(this->item == v.item); }
    bool IncidentTo(Vertex* v) { return this == v->p; }
    bool IncidentFrom(Vertex* v) { return v == p; }
    void ShortestPath(Vertex* v, std::vector<Vertex*>&);
    static void Reset(Vertex*, bool);

    Status s;
    int dist;     // Distance        -> Graph::Breadth()
    int t_found;  // Time found      -> Graph::Depth()
    int t_disc;   // Time discovered -> Graph::Depth()
    Vertex* p; // Predecessor
    Vertex* next;
    Vertex* prev;
};

template <typename I>
void Vertex<I>::ShortestPath(Vertex* v, std::vector<Vertex*>& path)
{
    if (v == this) {
        path.push_back(this);
    }
    else if (v == nullptr) {
        path.clear();
    }
    else {
        ShortestPath(v->p, path);
        if (!path.empty()) {
            path.push_back(v);
        }
    }
}

template <typename I>
void Vertex<I>::Reset(Vertex* v, bool source)
{
    if (source) {
        v->s = Vertex::Status::f;
        v->dist = 0;
        v->p = nullptr;
    }
    else {
        v->dist = 100000;
        v->s = Vertex::Status::nf;
        v->t_disc = 0;
        v->t_found = 0;
        v->p = nullptr;
    }
}

template <typename T>
using V = Vertex<T>;

template <typename I>
class GraphList;

template <typename I>
class Vertices {
public:
    using List = GraphList<I>;
    using Vertex = Vertex<I>;
    using Edges = std::unordered_map<Vertex*, List>;

    Vertices(int t)
        : set{}, total{ t + 1 }, count{}
    {
        edges[nullptr]; // Signals non-membership of a queried vertex.
    }
    Vertices(Vertices&& v) noexcept;
    List& operator[](Vertex*);
    auto begin() { return set.begin(); }
    auto end() { return set.end(); }

    void AddRelations(Vertex*, const std::vector<I>&);
    void AddVertex(Vertex*, const std::vector<I>&);
    void RemoveRelation(Vertex* source, Vertex* relation);
    void RemoveVertex(Vertex*);
    void ShortestPath(Vertex* s, Vertex* v, std::vector<Vertex*>&);
    void Transpose();
    Vertex* Search(const I&);
    int Size() { return set.size(); }

    std::vector<Vertex*> set;

private:
    Edges edges;
    int total;  // == |edges|
    int count;  // Running total.
};

template <typename I>
Vertices<I>::Vertices(Vertices&& v) noexcept
    : edges{ std::move(v.edges) }, set{ std::move(v.set) }, total{ v.total }, count{ v.count }
{
    v.total = 0;
    v.count = 0;
}

template <typename I>
GraphList<I>& Vertices<I>::operator[](Vertex* v)
{
    try {
        return edges.at(v);
    }
    catch (std::out_of_range& /*re*/) {
        if (count < total) {
            return edges[v];
        }
        else {
            return edges.at(nullptr);
        }
    };
}

template <typename I>
void Vertices<I>::AddRelations(Vertex* v, const std::vector<I>& incidentals)
{
    if (!incidentals.empty()) {
        ++count;
        for (I item : incidentals) {
            edges[v].Insert(I{ item });
        }
    }
}

template <typename I>
void Vertices<I>::AddVertex(Vertex* v, const std::vector<I>& incidentals)
{
    AddRelations(v, incidentals);
    edges[v].set = &set;
    set.push_back(v);
}

template <typename I>
void Vertices<I>::RemoveRelation(Vertex* s, Vertex* v)
{
    edges[s].RemoveRelation(v);
}

template <typename I>
void Vertices<I>::RemoveVertex(Vertex* v)
{
    if (edges.erase(v)) {
        for (auto u : set) {
            if (u != v) {
                RemoveRelation(u, v);
            }
        }
        set.erase(std::remove_if(set.begin(), set.end(),
            [&](Vertex* u) { return u == v; }), set.end());
        --total;
    }
}

template <typename I>
void Vertices<I>::ShortestPath(Vertex* s, Vertex* v, std::vector<Vertex*>& path)
{
    s->ShortestPath(Search(v->item), path);
}

template <typename I>
void Vertices<I>::Transpose()
{
    std::unordered_map<Vertex*, List> edges_t;
    edges_t[nullptr];
    for (auto k : set) {        // Accounts for vertices with only incident edges (directed graphs)
        edges_t[k].set = &set; //<-and initializes all 'set' pointers.
        for (Vertex* v : edges[k]) {
            edges_t[v].Insert(I{ k->item });
        }
    }
    edges = std::move(edges_t);
}

template <typename I>
V<I>* Vertices<I>::Search(const I& item)
{
    for (auto v : set) {
        if (v->item == item) {
            return v;
        }
    }
    return nullptr;
}

template <typename I>
struct Summary {
    using Vertex = Vertex<I>;
    using string = std::string;

    Summary(std::vector<Vertex*>& vertices, std::ostream& os = std::cout)
        : vertices{ vertices }, os{ os },
          header{ "Vertex", "Distance", "Time Found", "Time Discovered", "Parent", "Status" },
          statuses{ "Not Found", "Found", "Discovered" }
    {
    }

    void Print() const {}
    string String() const {}
    void ShortestPath(std::vector<Vertex*>&, std::ostream&);

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
    string status{};
    switch (v->s) {
    case Vertex::Status::nf: {
        status = "Not Found";
        break;
    }
    case Vertex::Status::f: {
        status = "Found";
        break;
    }
    case Vertex::Status::d: {
        status = "Discovered";
        break;
    }}

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

template <typename I>
void Summary<I>::ShortestPath(std::vector<Vertex*>& path, std::ostream& os)
{
    char beg = '<';
    char end = '>';
    char sep = ',';
    char wsp = ' ';
    if (path.empty()) {
        os << std::string{ beg + wsp + end };
    }
    else {
        std::ostringstream oss{};
        oss << beg;
        for (auto v : path) {
            oss << wsp << SafeStringConversion(v)
                << (v != path.back() ? sep : wsp);
        }
        os << oss.str() + end;
    }
}