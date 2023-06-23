#pragma once

#include <map>
#include <vector>

template <typename T>
class LinkedList {
   public:
    struct Node {
        T value{};
        Node* next{nullptr};
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
    if (_head == nullptr) {
        _head = new Node;
        _head->value = value;
    } else {
        Node* p_new_node = new Node;
        p_new_node->value = value;

        p_new_node->next = _head;
        _head = p_new_node;
    }
}

template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& value) {
    if (node == nullptr || _head == nullptr) {
        PushFront(value);
        return;
    }

    if (_head == nullptr) {
        return;
    }

    Node* p_new_node = new Node;
    p_new_node->value = value;
    Node* p_current = _head;

    while (p_current != node && p_current->next != nullptr) {
        p_current = p_current->next;
    }

    if (p_current->next == nullptr) {
        p_current->next = p_new_node;
        return;
    }

    p_new_node->next = p_current->next;
    p_current->next = p_new_node;
}

template <typename T>
void LinkedList<T>::RemoveAfter(Node* node) {
    if (_head == nullptr) {
        return;
    }

    if (node == nullptr) {
        PopFront();
    }

    Node* p_current = _head;
    while (p_current != node && p_current->next != nullptr) {
        p_current = p_current->next;
    }

    if (p_current->next == nullptr) {
        return;
    }

    Node* p_to_delete = p_current->next;
    p_current->next = p_to_delete->next;
    delete p_to_delete;
}

template <typename T>
void LinkedList<T>::PopFront() {
    if (_head == nullptr) {
        return;
    }

    Node* p_next_to_head = _head->next;
    delete _head;
    _head = p_next_to_head;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    if (_head == nullptr) {
        return;
    }

    for (Node* p_node_to_delete = _head; _head != nullptr; p_node_to_delete = _head) {
        _head = _head->next;
        delete p_node_to_delete;
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