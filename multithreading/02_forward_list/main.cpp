#include <thread>
#include <vector>
#include <mutex>

class ForwardList {
  public:
    void Push() {
        Node* new_node = new Node();

        {  // ===== critical section =====
            std::lock_guard<std::mutex> guard(mu_);
            new_node->next = head_;
            head_ = new_node;
        }  // <== mu_.unlock()
    }

    void Pop() {
        std::lock_guard<std::mutex> guard(mu_);

        // ===== critical section =====
        Node* ptr_to_head = head_;
        head_ = ptr_to_head->next;
        delete ptr_to_head;
    }  // <== mu_.unlock()

  private:
    struct Node {
        Node* next;
    };

    std::mutex mu_;
    Node* head_{nullptr};
};

int main() {
    ForwardList list;

    // Just works
    list.Push();
    list.Push();
    list.Push();
    list.Pop();
    list.Pop();
    list.Push();
    list.Pop();
    list.Pop();
 // ================ Concurrent access ================
    std::vector<std::thread> threads;
    for (size_t i = 0; i < 5; ++i) {
        threads.emplace_back([&]() {
            for (size_t k = 0; k < 100500; ++k) {
                list.Push();
                list.Pop();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
