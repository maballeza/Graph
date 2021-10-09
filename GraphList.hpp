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
        Vertex* operator*();
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
    Vertex* v = List<V, I>::Iterator::operator*();
    if (v) {
        g->Normalize(&v, g);
    }
    return v;
}

template <typename I>
typename GraphList<I>::Iterator GraphList<I>::begin()
{
    auto It = List<V, I>::begin();
    return Iterator{ *It, this };
}

template <typename I>
typename GraphList<I>::Iterator GraphList<I>::end()
{
    auto It = List<V, I>::end();
    return Iterator{ *It, this };
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
