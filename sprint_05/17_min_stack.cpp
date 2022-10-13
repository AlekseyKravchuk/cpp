#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
class StackMin {
   public:
    void Push(const Type& element) {
        elements_.push_back(element);
        elementsMultiSet_.insert(element);
    }
    void Pop() {
        auto elm = Peek();
        elements_.pop_back();
        elementsMultiSet_.erase(elementsMultiSet_.lower_bound(elm));
    }
    const Type& Peek() const {
        return elements_.back();
    }
    Type& Peek() {
        return elements_.back();
    }

    const Type& PeekMin() const {
        return *(elementsMultiSet_.begin());
    }

    void Print() const {
        bool isFirstElement = true;
        for (auto elm : elements_) {
            if (isFirstElement) {
                cout << elm;
                isFirstElement = false;
            } else {
                cout << " "s << elm;
            }
        }
        cout << endl;
    }

    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.empty();
    }

   private:
    vector<Type> elements_;
    multiset<Type> elementsMultiSet_;
};

int main() {
    StackMin<int> stack;
    vector<int> values(5);
    // заполняем вектор для тестирования нашего стека
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_shuffle(values.begin(), values.end());
    // заполняем стек
    for (int i = 0; i < 5; ++i) {
        stack.Push(values[i]);
    }
    // печатаем стек и его минимум, постепенно убирая из стека элементы
    while (!stack.IsEmpty()) {
        stack.Print();
        cout << "Минимум = "s << stack.PeekMin() << endl;
        stack.Pop();
    }

}