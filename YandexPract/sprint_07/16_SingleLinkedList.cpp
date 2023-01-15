#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename T>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const T& val, Node* next) : value(val), next_node(next) { }
        T value;
        Node* next_node = nullptr;
    };

public:
    SingleLinkedList() = default;

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

private:
    Node head_ = Node();  // Фиктивный узел, используется для вставки "перед первым элементом"
    size_t size_ = 0;
};

void Test0() {
    using namespace std;
    {
        const SingleLinkedList<int> empty_int_list;
        assert(empty_int_list.GetSize() == 0u);
        assert(empty_int_list.IsEmpty());
    }

    {
        const SingleLinkedList<string> empty_string_list;
        assert(empty_string_list.GetSize() == 0u);
        assert(empty_string_list.IsEmpty());
    }
}

int main() {
    Test0();
}