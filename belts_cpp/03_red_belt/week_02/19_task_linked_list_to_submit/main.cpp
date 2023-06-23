#include <string>
#include <vector>

#include "test_runner.h"

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
    std::set<Node*> _nodes_pointers;
};

template <typename T>
void LinkedList<T>::PushFront(const T& value) {
    if (_head == nullptr) {
        _head = new Node;
        _head->value = value;

        // =====================
        // _nodes_pointers.insert(_head);
    } else {
        Node* p_new_node = new Node;
        p_new_node->value = value;

        p_new_node->next = _head;
        _head = p_new_node;
        // _nodes_pointers.insert(p_new_node);  // <=====================
    }
}

template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& value) {
    if (node == nullptr || _head == nullptr) {
        PushFront(value);
        return;
    }

    Node* p_new_node = new Node;
    p_new_node->value = value;
    // _nodes_pointers.insert(p_new_node);  // <=====================

    // if (!_nodes_pointers.count(node)) {  // <=====================
    //     return;
    // }

    if (node->next == nullptr) {
        node->next = p_new_node;
        return;
    }

    p_new_node->next = node->next;
    node->next = p_new_node;
}

template <typename T>
void LinkedList<T>::RemoveAfter(Node* node) {
    if (_head == nullptr) {
        return;
    }

    if (node == nullptr) {
        PopFront();
    }

    // if (!_nodes_pointers.count(node)) {  // <=====================
    //     return;
    // }

    if (node->next == nullptr) {
        return;
    }

    Node* p_to_delete = node->next;
    node->next = p_to_delete->next;

    // // =====================
    // _nodes_pointers.erase(p_to_delete);
    // // =====================

    delete p_to_delete;
}

template <typename T>
void LinkedList<T>::PopFront() {
    if (_head == nullptr) {
        return;
    }

    Node* p_next_to_head = _head->next;
    // // =====================
    // _nodes_pointers.erase(_head);
    // // =====================

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

void TestPushFront() {
    LinkedList<int> list;

    list.PushFront(1);
    ASSERT_EQUAL(list.GetHead()->value, 1);
    list.PushFront(2);
    ASSERT_EQUAL(list.GetHead()->value, 2);
    list.PushFront(3);
    ASSERT_EQUAL(list.GetHead()->value, 3);

    const std::vector<int> expected = {3, 2, 1};
    ASSERT_EQUAL(ToVector(list), expected);
}

void TestInsertAfter() {
    LinkedList<std::string> list;

    list.PushFront("a");
    auto head = list.GetHead();
    ASSERT(head);
    ASSERT_EQUAL(head->value, "a");

    list.InsertAfter(head, "b");
    const std::vector<std::string> expected1 = {"a", "b"};
    ASSERT_EQUAL(ToVector(list), expected1);

    list.InsertAfter(head, "c");
    const std::vector<std::string> expected2 = {"a", "c", "b"};
    ASSERT_EQUAL(ToVector(list), expected2);
}

void TestRemoveAfter() {
    LinkedList<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.PushFront(i);
    }

    const std::vector<int> expected = {5, 4, 3, 2, 1};
    ASSERT_EQUAL(ToVector(list), expected);

    auto next_to_head = list.GetHead()->next;
    list.RemoveAfter(next_to_head);  // удаляем 3
    list.RemoveAfter(next_to_head);  // удаляем 2

    const std::vector<int> expected1 = {5, 4, 1};
    ASSERT_EQUAL(ToVector(list), expected1);

    while (list.GetHead()->next) {
        list.RemoveAfter(list.GetHead());
    }
    ASSERT_EQUAL(list.GetHead()->value, 5);
}

void TestPopFront() {
    {
        LinkedList<int> list;

        for (int i = 1; i <= 5; ++i) {
            list.PushFront(i);
        }
        for (int i = 1; i <= 5; ++i) {
            list.PopFront();
        }
        ASSERT(list.GetHead() == nullptr);
    }

    {
        LinkedList<int> list;
        list.PushFront(22);
        list.PopFront();
        list.PopFront();

        ASSERT(list.GetHead() == nullptr);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestPushFront);
    RUN_TEST(tr, TestInsertAfter);
    RUN_TEST(tr, TestRemoveAfter);
    RUN_TEST(tr, TestPopFront);
    return 0;
}
