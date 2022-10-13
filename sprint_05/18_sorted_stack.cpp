#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    if (beginIt == endIt) {
        cout << endl;
        return;
    }
    
    for (auto it = beginIt; it != endIt; ++it) {
        if (next(it) != endIt) {
            cout << *it << " "s;
        } else {
            cout << *it << endl;
        }
    }
}

template <typename Type>
class Stack {
   public:
    void Push(const Type& element) {
        elements_.push_back(element);
    }
    void Pop() {
        elements_.pop_back();
    }
    const Type& Peek() const {
        return elements_.back();
    }
    Type& Peek() {
        return elements_.back();
    }
    void Print() const {
        PrintRange(elements_.begin(), elements_.end());
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.empty();
    }

   private:
    vector<Type> elements_;
};

template <typename Type>
class SortedStack {
   public:
    void Push(const Type& element) {
        if (elements_.IsEmpty() || element <= elements_.Peek()) {
            elements_.Push(element);
        } else {
            Stack<Type> tmp;

            while (element > elements_.Peek()) {
                if (!elements_.IsEmpty()) {
                    tmp.Push(elements_.Peek());
                    elements_.Pop();
                } else {
                    break;
                }
            }

            elements_.Push(element);

            while (!tmp.IsEmpty()) {
                elements_.Push(tmp.Peek());
                tmp.Pop();
            }
        }
    }

    void Pop() {
        elements_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek();
    }
    Type& Peek() {
        return elements_.Peek();
    }
    void Print() const {
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.Size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }

   private:
    Stack<Type> elements_;
};

int main() {
    SortedStack<int> stack;
    cout << "size = " << stack.Size() << endl;
    vector<int> values = {5, 4, 2};

    for (size_t i = 0; i < values.size(); ++i) {
        cout << "Вставляемый элемент = "s << values[i] << endl;
        stack.Push(values[i]);
        stack.Print();
    }

    int elementToInsert1 = 4;
    cout << "Вставляемый элемент = "s << elementToInsert1 << endl;
    stack.Push(elementToInsert1);
    stack.Print();
    cout << "size = "s << stack.Size() << endl
         << endl;

    int elementToInsert2 = 6;
    cout << "Вставляемый элемент = "s << elementToInsert2 << endl;
    stack.Push(elementToInsert2);
    stack.Print();
    cout << "size = "s << stack.Size() << endl
         << endl;

    cout << "Before poping element: ";
    stack.Print();
    cout << "size = "s << stack.Size() << endl
         << endl;

    cout << "AFTER poping element: ";
    stack.Pop();
    stack.Print();
    cout << "size = "s << stack.Size() << endl
         << endl;

    
    stack.Pop();
    cout << "AFTER poping element: ";
    stack.Print();
    cout << "size = "s << stack.Size() << endl
         << endl;

    
    stack.Pop();
    cout << "AFTER poping element: ";
    stack.Print();
    cout << "size = "s << stack.Size() << endl
         << endl;

    
    stack.Pop();
    cout << "AFTER poping element: ";
    stack.Print();
    cout << "size = "s << stack.Size() << endl
         << endl;

    
    stack.Pop();
    cout << "AFTER poping element: ";
    stack.Print();
    cout << "size = "s << stack.Size() << endl
         << endl;
}