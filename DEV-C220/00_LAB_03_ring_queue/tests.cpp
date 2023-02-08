#include <cassert>
#include <iostream>
#include <sstream>  // std::istringstream, std::ostringstream
#include <string>

#include "../my_print.h"
#include "ring_queue.h"

using namespace std::literals;

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
    q.PrintRingQueueState();

    q.pop();
    PrintCollection(q, "q after 1-st pop(): ");
    q.PrintRingQueueState();

    q.pop();
    PrintCollection(q, "q after 2-nd pop(): ");
    q.PrintRingQueueState();

    q.push("55"s);
    PrintCollection(q, "q after push(55): ");
    q.PrintRingQueueState();

    q.push("66"s);
    PrintCollection(q, "q after push(66): ");
    q.PrintRingQueueState();

    q.push("77"s);
    PrintCollection(q, "q after push(77): ");
    q.PrintRingQueueState();
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

void TEST_RingQueue_CopyConstructor() {
    RingQueue<std::string> q1{"11"s, "22"s, "33"s, "44"};
    q1.pop();
    q1.pop();
    q1.push("55");  // EMPTY EMPTY "33 "44" "55", _first = 2, _last = 0,
    q1.push("66");  // "66"  EMPTY "33 "44" "55", _first = 2, _last = 1

    RingQueue<std::string> q2 = q1;
    assert(q1 == q2);
    std::cout << "TEST_RingQueue_CopyConstructor: PASSED"s << std::endl;
}

void Print_TEST_RingQueue_CopyConstructor() {
    RingQueue<std::string> q1{"11"s, "22"s, "33"s, "44"};
    q1.pop();
    q1.pop();
    q1.push("55");  // EMPTY EMPTY "33 "44" "55", _first = 2, _last = 0,
    PrintCollection(q1, "q1 after push(55): ");
    q1.PrintRingQueueState();

    q1.push("66");  // "66"  EMPTY "33 "44" "55", _first = 2, _last = 1
    PrintCollection(q1, "q1 after manipulations: ");
    q1.PrintRingQueueState();

    RingQueue<std::string> q2 = q1;
    PrintCollection(q2, "q2 after CopyConstructoring: ");
    q2.PrintRingQueueState();
}

void TEST_RingQueue_ConstructorWithDefaultValues() {
    RingQueue<std::string> q1(10, "!"s);
    assert(q1 == (RingQueue<std::string>{"!"s, "!"s, "!"s, "!"s, "!"s, "!"s, "!"s, "!"s, "!"s, "!"s}));
    std::cout << "TEST_RingQueue_ConstructorWithDefaultValues: PASSED"s << std::endl;
}

void TEST_RingQueue_AssignmentOperator() {
    RingQueue<std::string> q1{"11"s, "22"s, "33"s, "44"s};
    q1.pop();
    q1.pop();
    q1.push("55");  // EMPTY EMPTY "33 "44" "55", _first = 2, _last = 0,
    q1.push("66");  // "66"  EMPTY "33 "44" "55", _first = 2, _last = 1

    RingQueue<std::string> q3{"111"s, "222"s, "333"s, "444"s, "555"s, "666"s, "777"s};
    q3.pop();
    q3.pop();
    q3.pop();
    q3.pop();
    q3.push("888"s);
    q3.push("999"s);  //    

    RingQueue<std::string> q2 = q1;
    q2 = q3;
    assert(q3 == q2);
    std::cout << "TEST_RingQueue_AssignmentOperator: PASSED"s << std::endl;
}

void Print_TEST_RingQueue_AssignmentOperator() {
    RingQueue<std::string> q1{"11"s, "22"s, "33"s, "44"s};
    q1.pop();
    q1.pop();
    q1.push("55");  // EMPTY EMPTY "33 "44" "55", _first = 2, _last = 0,
    q1.push("66");  // "66"  EMPTY "33 "44" "55", _first = 2, _last = 1

    RingQueue<std::string> q3{"111"s, "222"s, "333"s, "444"s, "555"s, "666"s, "777"s};
    q3.pop();
    q3.pop();
    q3.pop();
    q3.pop();
    q3.push("888"s);
    q3.push("999"s);  // "999" EMPTY EMPTY EMPTY "555" "666" "777" "888" _first = 4, _last = 2, size = 5
    PrintCollection(q3, "q3: ");
    q3.PrintRingQueueState();

    RingQueue<std::string> q2 = q1;
    q2 = q3;
    assert(q3 == q2);

    PrintCollection(q2, "q2 after assignment (q2 = q3): ");
    q2.PrintRingQueueState();
}

void TEST_RingQueue_MovingCopyConstructor() {
    RingQueue<std::string> q1{"11"s, "22"s, "33"s, "44"s};
    q1.pop();
    q1.pop();
    q1.push("55");  // EMPTY EMPTY "33 "44" "55", _first = 2, _last = 0,
    q1.push("66");  // "66"  EMPTY "33 "44" "55", _first = 2, _last = 1

    RingQueue<std::string> q2 = q1;
    RingQueue<std::string> q3 = std::move(q2);
    assert((q2.isEmpty() && (q3 == q1)));
    std::cout << "TEST_RingQueue_MovingCopyConstructor: PASSED"s << std::endl;
}

void Print_TEST_RingQueue_MovingCopyConstructor() {
    RingQueue<std::string> q1{"11"s, "22"s, "33"s, "44"s};
    q1.pop();
    q1.pop();
    q1.push("55");  // EMPTY EMPTY "33 "44" "55", _first = 2, _last = 0,
    q1.push("66");  // "66"  EMPTY "33 "44" "55", _first = 2, _last = 1
    PrintCollection(q1, "q1 after manipulations: ");
    q1.PrintRingQueueState();

    RingQueue<std::string> q2 = q1;
    PrintCollection(q2, "q2 after (q2 = q1): ");
    q2.PrintRingQueueState();

    RingQueue<std::string> q3 = std::move(q2);
    assert((q2.isEmpty() && (q3 == q1)));
    PrintCollection(q3, "q3 after (q3 = std::move(q2)): ");
    q3.PrintRingQueueState();

    PrintCollection(q2, "q2 after (q3 = std::move(q2)): ");
    q2.PrintRingQueueState();
}

void TEST_RingQueue_MovingAssignmentOperator() {
    RingQueue<std::string> q1{"11"s, "22"s, "33"s, "44"s};
    q1.pop();
    q1.pop();
    q1.push("55");  // EMPTY EMPTY "33 "44" "55", _first = 2, _last = 0,
    q1.push("66");  // "66"  EMPTY "33 "44" "55", _first = 2, _last = 1

    RingQueue<std::string> q2{"aaa"s, "bbb"s, "ccc"s, "ddd"s, "eee"s, "fff"s};
    q1 = std::move(q2);
    assert(q2.isEmpty() && (q1 == RingQueue<std::string>{"aaa"s, "bbb"s, "ccc"s, "ddd"s, "eee"s, "fff"s}));
    std::cout << "TEST_RingQueue_MovingAssignmentOperator: PASSED"s << std::endl;
}

void Print_TEST_RingQueue_MovingAssignmentOperator() {
    RingQueue<std::string> q1{"11"s, "22"s, "33"s, "44"s};
    q1.pop();
    q1.pop();
    q1.push("55");  // EMPTY EMPTY "33 "44" "55", _first = 2, _last = 0,
    q1.push("66");  // "66"  EMPTY "33 "44" "55", _first = 2, _last = 1

    RingQueue<std::string> q2{"aaa"s, "bbb"s, "ccc"s, "ddd"s, "eee"s, "fff"s};
    PrintCollection(q2, "q2: ");
    q2.PrintRingQueueState();

    q1 = std::move(q2);
    assert(q2.isEmpty() && (q1 == RingQueue<std::string>{"aaa"s, "bbb"s, "ccc"s, "ddd"s, "eee"s, "fff"s}));
    PrintCollection(q1, "q1 after (q1 = std::move(q2)): ");
    q1.PrintRingQueueState();

    PrintCollection(q2, "q1 after (q1 = std::move(q2)): ");
    q2.PrintRingQueueState();
}