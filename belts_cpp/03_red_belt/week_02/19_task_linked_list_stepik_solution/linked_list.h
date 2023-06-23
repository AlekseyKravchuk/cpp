#pragma once

#include <set>
#include <vector>

template <typename T>
class LinkedList {
   public:
    struct Node {
        T value;
        Node* next = nullptr;
    };

    ~LinkedList();

    void PushFront(const T& value);
    void InsertAfter(Node* node, const T& value);
    void RemoveAfter(Node* node);
    void PopFront();

    Node* GetHead() { return _head; }
    const Node* GetHead() const { return _head; }

   private:
    Node* _head = nullptr;
};

template <typename T>
LinkedList<T>::~LinkedList() {
    while (_head) {
        PopFront();
    }
}

template <typename T>
void LinkedList<T>::PushFront(const T& value) {
    Node* node = new Node{value};
    node->next = _head;
    _head = node;
}

template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& value) {
    if (node) {
        auto new_node = new Node{value};
        new_node->next = node->next;
        node->next = new_node;
    } else {
        PushFront(value);
    }
}

template <typename T>
void LinkedList<T>::RemoveAfter(Node* node) {
    if (!node) {
        PopFront();
    } else if (node->next) {
        Node* removing_node = node->next;
        node->next = removing_node->next;
        delete removing_node;
    }
}

template <typename T>
void LinkedList<T>::PopFront() {
    if (_head) {
        Node* new_head = _head->next;
        delete _head;
        _head = new_head;
    }
}

template <typename T>
std::vector<T> ToVector(const LinkedList<T>& list) {
    std::vector<T> result;

    for (auto node = list.GetHead(); node; node = node->next) {
        result.push_back(node->value);
    }

    return result;
}