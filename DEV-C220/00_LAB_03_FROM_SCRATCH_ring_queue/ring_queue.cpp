#include "ring_queue.h"

#include <iostream>
#include <string>
#include <sstream>  // std::istringstream, std::ostringstream
#include <cassert>

#include "../my_print.h"

using namespace std::literals;

template <typename T>
void PrintRingQueueState(const RingQueue<T>& q) {
    std::cout << "_first = " << q.GetFirst() << ", "s
              << "_last = " << q.GetLast() << ", "s
              << "_size = " << q.GetSize() << ", "s
              << "_capacity = " << q.GetCapacity() << ", "s
              << std::endl;
    std::cout << std::string(20, '=') << std::endl;
}

void TEST_RingQueue_PushPop() {
    RingQueue<std::string> q = {"11"s, "22"s, "33"s, "44"s};
    assert(q.GetFirst() == 0 && q.GetLast() == 4 && q.GetSize() == 4 && q.GetCapacity() == 5);

    auto poppedVal = q.pop();
    assert(poppedVal == "11"s);
    assert(q.GetFirst() == 1 && q.GetLast() == 4 && q.GetSize() == 3 && q.GetCapacity() == 5);

    q.pop();
    assert(q.GetFirst() == 2 && q.GetLast() == 4 && q.GetSize() == 2 && q.GetCapacity() == 5);

    q.push("55"s);
    assert(q.GetFirst() == 2 && q.GetLast() == 0 && q.GetSize() == 3 && q.GetCapacity() == 5);

    q.push("66"s);
    assert(q.GetFirst() == 2 && q.GetLast() == 1 && q.GetSize() == 4 && q.GetCapacity() == 5);

    q.push("77"s);
    assert(q.GetFirst() == 0 && q.GetLast() == 5 && q.GetSize() == 5 && q.GetCapacity() == 7);

    std::cout << "TEST_RingQueue_PushPop: PASSED"s << std::endl;
}

void Print_TEST_RingQueue_PushPop() {
    RingQueue<std::string> q = {"11"s, "22"s, "33"s, "44"s};
    PrintCollection(q, "q initial state: "s);
    PrintRingQueueState(q);

    q.pop();
    PrintCollection(q, "q after 1-st pop(): ");
    PrintRingQueueState(q);

    q.pop();
    PrintCollection(q, "q after 2-nd pop(): ");
    PrintRingQueueState(q);

    q.push("55"s);
    PrintCollection(q, "q after push(55): ");
    PrintRingQueueState(q);

    q.push("66"s);
    PrintCollection(q, "q after push(66): ");
    PrintRingQueueState(q);

    q.push("77"s);
    PrintCollection(q, "q after push(77): ");
    PrintRingQueueState(q);
}

void TEST_RingQueue_RangeBasedForLoop() {
    // тест использования RingQueue в диапазонном for:
    RingQueue<std::string> q{"AAA"s, "qwerty"s, "bbb"s, "OTHER"};
    std::ostringstream oss;
    q.pop();
    q.pop();
    q.push("STOP"s);

    bool isFirst = true;
    for (const auto& el : q) {
        if (isFirst) {
            oss << el;
            isFirst = false;
        } else {
            oss << " "s << el;
        }
        
    }
    std::ostringstream ossTest("bbb OTHER STOP"s);
    assert(oss.str() == ossTest.str());
    std::cout << "TEST_RingQueue_RangeBasedForLoop: PASSED"s << std::endl;
}

void Print_TEST_RingQueue_RangeBasedForLoop() {
    // использование RingQueue в диапазонном for:
    RingQueue<std::string> q{"AAA"s, "qwerty"s, "bbb"s, "OTHER"};

    bool isFirst = true;
    for (const auto& el : q) {
        if (isFirst) {
            std::cout << el;
            isFirst = false;
        } else {
            std::cout << " "s << el;
        }
        
    }
    std::cout << std::endl;
}

void TEST_RingQueue_ClassicConstructor() {
    RingQueue<std::string> q1{"11"s, "22"s, "33"s, "44"};
    q1.pop();
    q1.pop();
    q1.push("55"); // EMPTY EMPTY "33 "44" "55"
    PrintCollection(q1, "q1 after manipulations: ");
    PrintRingQueueState(q1);
    RingQueue<std::string> q2 = q1;
    // std::cout << "TEST_RingQueue_ClassicConstructor: PASSED"s << std::endl;
}

int main() {
    // TEST_RingQueue_PushPop();
    // Print_TEST_RingQueue_PushPop();

    // TEST_RingQueue_RangeBasedForLoop();
    // Print_TEST_RingQueue_RangeBasedForLoop();

    TEST_RingQueue_ClassicConstructor();


    return 0;
}
