#include "Vertices.hpp"
#include "List.hpp"
#include <iostream> // Exception-handling.

template <typename I>
class GraphList : public List<V, I> {
public:
    using Vertex = V<I>;
    using List<V, I>::List;

    struct Exception {
        const char* what() const { return "GraphList: Invariant violation!\n"; }
    };
    struct Iterator : List<V, I>::Iterator {
        using List<V, I>::Iterator::Iterator;
        Vertex* operator*();
    };

    Iterator begin();
    Iterator end();
    
    Vertex* Search(const I& i);
    static void Normalize(Vertex**);

    inline static std::vector<Vertex*>* set{};
};

template <typename I>
V<I>* GraphList<I>::Iterator::operator*()
{
    Vertex* v = List<V, I>::Iterator::operator*();
    if (v) {
        Normalize(&v);
    }
    return v;
}

template <typename I>
typename GraphList<I>::Iterator GraphList<I>::begin()
{
    auto It = List<V, I>::begin();
    return Iterator{ *It };
}

template <typename I>
typename GraphList<I>::Iterator GraphList<I>::end()
{
    auto It = List<V, I>::end();
    return Iterator{ *It };
}

template <typename I>
V<I>* GraphList<I>::Search(const I& i)
{
    Vertex* index = List<V, I>::Search(i);
    if (index) {
        Normalize(&index);
    }
    return index;
}

template <typename I>
void GraphList<I>::Normalize(Vertex** list_v)
{
    if (set) {
        for (auto v : *set) {
            if (v->item == (*list_v)->item) {
                *list_v = v;
                return;
            }
        }
    }
    else {
        throw Exception{};
    }
}
