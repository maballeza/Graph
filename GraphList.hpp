#include "Vertices.hpp"
#include "List.hpp"

template <typename I>
class GraphList : public List<V, I> {
public:
    using Vertex = V<I>;
    using List<V, I>::List;
    using list_iterator = typename List<V, I>::Iterator;

    struct set_uninitialized
    {};
    struct Iterator : list_iterator {
        explicit Iterator(Vertex* v, GraphList* g);
        Vertex* operator*(); // Deliberate departure from convention.
        GraphList* g;
    };

    Iterator begin();
    Iterator end();
    
    Vertex* Search(const I& i);
    void Normalize(Vertex**, GraphList* g);

    std::vector<Vertex*>* set{};
};

template <typename I>
GraphList<I>::Iterator::Iterator(Vertex* v, GraphList* g)
    : list_iterator{ v }, g{ g }
{}

template <typename I>
V<I>* GraphList<I>::Iterator::operator*()
{
    Vertex v = List<V, I>::Iterator::operator*();
    Vertex* w = &v;
    g->Normalize(&w, g);
    return w;
}

template <typename I>
typename GraphList<I>::Iterator GraphList<I>::begin()
{
    Vertex* v = &List<V, I>::begin();
    return Iterator{ v, this };
}

template <typename I>
typename GraphList<I>::Iterator GraphList<I>::end()
{
    Vertex* v = &List<V, I>::end();
    return Iterator{ v, this };
}

template <typename I>
V<I>* GraphList<I>::Search(const I& i)
{
    Vertex* index = List<V, I>::Search(i);
    if (index) {
        Normalize(&index, this);
    }
    return index;
}

template <typename I>
void GraphList<I>::Normalize(Vertex** list_v, GraphList* g)
{
    if (set) {
        for (auto v : *g->set) {
            if (v->item == (*list_v)->item) {
                *list_v = v;
                return;
            }
        }
    }
    else {
        throw set_uninitialized{};
    }
}
