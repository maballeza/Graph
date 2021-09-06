#pragma once
#include "Node.hpp"

/**
* List
*   A doubly-/singly-linked list implementation.
*/
template <template <typename> class N, typename I>
class List {
public:
    using Node = N<I>;
    using Handle = HNode<N, I>;

    struct Iterator {
        explicit Iterator(Node* n) : np{ n } {}
        Node* operator*() { return np; }
        Iterator& operator++() { np = np->next; return *this; }
        bool operator!=(const Iterator& ni) { return np != ni.np; }
    private:
        Node* np;
    };

    List() : head{}, tail{}, size{} {}
    List(List&& l) noexcept;
    ~List();

    List& operator=(List&& l) noexcept;

    Node* operator[](int i) {
        return Search(I{ i });
    }

    /**
    *   Enables range-for loop usage.
    */
    Iterator begin() { return Iterator{ head }; }
    Iterator end() { return Iterator{ tail }; }
    /**
    *   Inserts values at the head of the list.
    */
    Node* Insert(I&& i);
    Node* Insert(Node* n);  // Build<N, I>::Instance() should be used to create a Node*.
    void Delete(Node** n);
    Handle Release(I&& i);

    Node* Search(const I& i);
    int Size() const { return size; }

private:
    Node* head;
    Node* tail;
    int size;
};

template <template <typename> class N, typename I>
List<N, I>::List(List&& l) noexcept : head{}, tail{}, size{} {
    if (Node* n = l.head) {
        head = n;
        size = l.size;
        l.head = nullptr;
        l.size = 0;
    }
}

template <template <typename> class N, typename I>
List<N, I>::~List() {
    if (Node* n = head) {
        while (Node* m = n) { // Deletes nodes beginning with the head and stopping at the tail.
             n = n->next;
             delete m;
        }
        head = nullptr;
        size = 0;
    }
}

template <template <typename> class N, typename I>
List<N, I>& List<N, I>::operator=(List&& l) noexcept {
    if (head = l.head) {
        tail = l.tail;
        size = l.size;
        l.head = nullptr;
        l.size = 0;
    }
    return *this;
}

template <template <typename> class N, typename I>
typename List<N, I>::Node* List<N, I>::Search(const I& i) {
    Node* n = head;
    if (n) {
        while (n != tail && n->item != i) {
            n = n->next;
        }
    }
    return n;
}

template <template <typename> class N, typename I>
typename List<N, I>::Node* List<N, I>::Insert(I&& i) {
    return Insert(Build<N, I>::Instance(std::forward<I>(i)).Release());
}

#ifndef FORWARD
template <template <typename> class N, typename I>
typename List<N, I>::Node* List<N, I>::Insert(Node* n) {
    if (n) {
        if (head) { // If the list is not empty, supplants the head; ...
            n->next = head;
            n->next->prev = n;
            head = n;
        }
        else { // ... otherwise initializes the list.
            head = n;
        }
        ++size;
    }
    return n;
}
#else
template <template <typename> class N, typename I>
typename List<N, I>::Node* List<N, I>::Insert(Node* n) {
    if (n) {
        if (head) { // If the list is not empty, supplants the head; ...
            n->next = head;
            head = n;
        }
        else { // ... otherwise initializes the list.
            head = n;
        }
        ++size;
    }
    return n;
}
#endif

template <template <typename> class N, typename I>
typename List<N, I>::Handle List<N, I>::Release(I&& i) {
    auto n = Search(i);
    Delete(&n);
    return Build<N, I>::Instance(std::forward<I>(i));
}

#ifndef FORWARD
template <template <typename> class N, typename I>
void List<N, I>::Delete(Node** n) {
    if (Node* m = *n) {
        if (m == head) {
            head = m->next;
        }
        else {
            m->prev->next = m->next;
        }
        if (m->next != tail) {
            m->next->prev = m->prev;
        }
        --size;
        delete* n;
        *n = nullptr;
    }
}
#else
template <template <typename> class N, typename I>
void List<N, I>::Delete(Node** n) {
    Node* m = head;
    if (m == *n) {
        head = m->next;
    }
    else {
        while (m->next != *n) {
            m = m->next;
        }
        m->next = m->next->next;
    }
    --size;
    delete* n;
    *n = nullptr;
}
#endif