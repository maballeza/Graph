#include "List.hpp"
 
template <template <typename> class V, typename I>
class GraphList : public List<V, I> {
public:
    using Vertex = V<I>;
    using List<V, I>::List;
    using List<V, I>::Iterator;

    Vertex* operator[](int i) { return this->operator[](i); }
    Vertex* Insert(I&& i) { return List<V, I>::Insert(std::forward<I>(i)); }
    int Size() const { return List<V, I>::Size(); }
    
    Iterator begin() const { return List<V, I>::begin(); }
    Iterator end() const { return List<V, I>::end(); }
};