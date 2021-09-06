#pragma once
#include "Node.hpp"

/**
* Queue Requirements:
*   Template parameter N should
*       possess a member "N* next" (forward lists),
*       and (optionally) also a member "N* prev" (bidirectional lists), 
*       and be allocated using Build::Instance()
*/
template <template <typename> class N, typename I>
class Queue {
public:
    using Node = N<I>;

    Queue() : head{}, tail{}, size{} {}
    Queue(Queue&&) noexcept;
    ~Queue(); // Deallocates any nodes in its possession.

    void Enqueue(Node* n);
    Node* Dequeue();        // Releases nodes.

    int Size() const { return size; }

private:
    void DeallocateQueue(Node** n);
    
    Node* head;  // Value-storing.
    Node* tail;  // The condition head == tail indicates an empty queue.
    int size;
};

template <template <typename> class N, typename I>
Queue<N, I>::Queue(Queue&& q) noexcept : head{ q.head }, tail{ q.tail }, size(q.size) {
    q.head = nullptr;
    q.size = 0;
}

template <template <typename> class N, typename I>
Queue<N, I>::~Queue() {
    if (head != tail) {
        DeallocateQueue(&head);
    }
    size = 0;
}

template <template <typename> class N, typename I>
void Queue<N, I>::Enqueue(Node* n) {
    if (n) {
        if (head == tail) { // Initialize the Queue.
            head = n;
            head->next = tail;
        }
        else if (head->next == tail) { // Inductive step.
            head->next = n;
        }
        else { // Search for the node preceding the tail.
            Node* m = head->next;
            while (m->next != tail) {
                m = m->next;
            }
            m->next = n;
        }
        n->next = tail;
        ++size;
    }
}

template <template <typename> class N, typename I>
typename Queue<N, I>::Node* Queue<N, I>::Dequeue() {
    if (Node* temp = head) {
        if (head = temp->next) {
            head->next = temp->next->next;
        }
        --size;
        return HNode{ temp }.Release();
    }
    else {
        return temp;
    }
}

#ifdef NDEBUG
template <template <typename> class N>
inline typename Queue<N, std::string>::Node* Queue<N, std::string>::Dequeue() {
    if (Node* temp = head) {
        if (head = head->next) {
            head->next = temp->next->next;
        }
        --size;
        return HNode{ temp }.Release();
    }
    else {
        return temp;
    }
}
#endif

template <template <typename> class N, typename I>
void Queue<N, I>::DeallocateQueue(Node** n) {
    if (Node* m = *n; m->next != nullptr) {
        DeallocateQueue(&m->next);
    }
    delete* n;
    *n = nullptr;
}