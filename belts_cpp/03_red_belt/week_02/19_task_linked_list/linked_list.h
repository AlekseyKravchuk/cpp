#pragma once

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
    void PopFront();
    void RemoveAfter(Node* node);

    Node* GetHead() { return _head; }
    const Node* GetHead() const { return _head; }

   private:
    Node* _head = nullptr;
};

template <typename T>
void LinkedList<T>::PushFront(const T& value) {
    if (!_head) {
        _head = new Node;
        _head->value = value;
    } else {
        Node* pnode = new Node;
        pnode->value = value;
        pnode->next = _head->next;
        _head = pnode;
    }
}

template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& value) {
    if (!node) {
        PushFront(value);
    } else {
        Node* current = _head->next;

        while (current != node || current != nullptr) {
            current = current->next;
        }

        Node* pnode = new Node;
        pnode->value = value;
        pnode->next = current->next;
        current->next = pnode;
    }
}

template <typename T>
void LinkedList<T>::PopFront() {
    if (!_head) {
        return;
    }

    Node* pnode_to_delete = _head->next;
    _head = pnode_to_delete->next;
    delete pnode_to_delete;
}

template <typename T>
void LinkedList<T>::RemoveAfter(Node* node) {
    if (node == nullptr) {
        PopFront();
    }

    if (node->next == nullptr) {
        return;
    }

    if (!_head) {
        return;
    }

    Node* current = _head->next;

    // TODO:
    while (current != node || current != nullptr) {
        current = current->next;
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