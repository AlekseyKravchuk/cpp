#include <cassert>
#include <stack>

using namespace std;

// моделируем очередь с помощью двух стеков
template <class T>
class QueueFromStack {
   public:
    void Push(const T& e) {
        _stack1.push(e);
    }

    T Pop() {
        if (_stack2.empty()) {
            while (!_stack1.empty()) {
                _stack2.push(_stack1.top());
                _stack1.pop();
            }
        }

        assert(!_stack2.empty());

        T top = _stack2.top();
        _stack2.pop();
        return top;
    }

   private:
    stack<T> _stack1;
    stack<T> _stack2;
};

int main() {
    QueueFromStack<int> q;
    q.Push(1);
    q.Push(2);
    q.Push(3);
    q.Push(4);
    assert(q.Pop() == 1);
    assert(q.Pop() == 2);
    assert(q.Pop() == 3);
    assert(q.Pop() == 4);
}