#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
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
class Queue {
   public:
    void Push(const Type& element) {
        _inbox.push(element);
    }

    // pop all the elements from "_inbox" stack and push popped elements to "_outbox" stack
    // ONLY in case if "_outbox" stack is empty
    void MoveAllFromInboxToOutbox() {
        if (_outbox.empty()) {
            while (!_inbox.empty()) {
                _outbox.push(_inbox.top());
                _inbox.pop();
            }
        }
    }

    void Pop() {
        MoveAllFromInboxToOutbox();
        _outbox.pop();
    }

    Type& Front() {
        MoveAllFromInboxToOutbox();
        return _outbox.top();
    }

    uint64_t Size() const {
        return (_inbox.size() + _outbox.size());
    }
    bool IsEmpty() const {
        return Size() == 0;
    }

   private:
    stack<Type> _inbox;
    stack<Type> _outbox;
};

int main() {
    Queue<int> queue;
    vector<int> values(5);
    // заполняем вектор для тестирования очереди
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_shuffle(values.begin(), values.end());
    PrintRange(values.begin(), values.end());
    cout << "Заполняем очередь"s << endl;
    // заполняем очередь и выводим элемент в начале очереди
    for (int i = 0; i < 5; ++i) {
        queue.Push(values[i]);
        cout << "Вставленный элемент "s << values[i] << endl;
        cout << "Первый элемент очереди "s << queue.Front() << endl;
    }
    cout << "Вынимаем элементы из очереди"s << endl;
    // выводим элемент в начале очереди и вытаскиваем элементы по одному
    while (!queue.IsEmpty()) {
        // сначала будем проверять начальный элемент, а потом вытаскивать,
        // так как операция Front на пустой очереди не определена
        cout << "Будем вынимать элемент "s << queue.Front() << endl;
        queue.Pop();
    }
    return 0;
}
