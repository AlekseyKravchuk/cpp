#include "ring_queue.h"
#include "../my_print.h"
#include "tests.h"

using namespace std::literals;

int main() {
    TEST_RingQueue_PushPop();
    // Print_TEST_RingQueue_PushPop();

    TEST_RingQueue_RangeBasedForLoop();
    // // Print_TEST_RingQueue_RangeBasedForLoop();

    TEST_RingQueue_CopyConstructor();
    // Print_TEST_RingQueue_CopyConstructor();

    TEST_RingQueue_ConstructorWithDefaultValues();

    TEST_RingQueue_AssignmentOperator();
    // // Print_TEST_RingQueue_AssignmentOperator();

    TEST_RingQueue_MovingCopyConstructor();
    // // Print_TEST_RingQueue_MovingCopyConstructor();

    TEST_RingQueue_MovingAssignmentOperator();
    // // Print_TEST_RingQueue_MovingAssignmentOperator();

    return 0;
}
