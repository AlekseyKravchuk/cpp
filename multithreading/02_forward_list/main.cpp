#include <iostream>
#include <thread>
#include <vector>

//#include "mutex.h"

class ForwardList {
  public:
    void Push() {
        Node* new_node = new Node();
        new_node->next = head_;
        head_ = new_node;
    }

    void Pop() {
        Node* head = head_;
        head_ = head->next;
        delete head;
    }

  private:
    struct Node {
        Node* next;
    };

    Node* head_{nullptr};
};

int main() {
    ForwardList list;

    // Just works
    list.Push();
    list.Push();
    list.Pop();
    list.Pop();
    list.Push();
    list.Pop();

//    std::vector<std::thread> threads;
//    for (size_t i = 0; i < 5; ++i) {
//        threads.emplace_back([&]() {
//            for (size_t k = 0; k < 100500; ++k) {
//                list.Push();
//                list.Pop();
//            }
//        });
//    }
//
//    for (auto& t : threads) {
//        t.join();
//    }

    return 0;
}
