#include "tests.h"

#include "deque.h"
#include "test_runner.h"

void Test_Empty() {
    {
        Deque<int> dq;
        ASSERT(dq.Empty());
    }
}

void Test_Size() {
    {
        Deque<int> dq;
        ASSERT_EQUAL(dq.Size(), 0u);
    }

    {
        Deque<int> dq;
        dq.PushFront(7);
        ASSERT_EQUAL(dq.Size(), 1u);
    }

    {
        Deque<int> dq;
        dq.PushFront(7);
        dq.PushBack(3);
        ASSERT_EQUAL(dq.Size(), 2u);
    }

    {
        Deque<int> dq;
        dq.PushFront(7);
        dq.PushFront(18);
        dq.PushFront(20);

        dq.PushBack(5);
        dq.PushBack(3);

        ASSERT_EQUAL(dq.Size(), 5u);
    }
}

void Test_Push() {
    {
        Deque<int> dq;
        dq.PushFront(7);
        dq.PushFront(18);
        dq.PushFront(20);

        dq.PushBack(5);
        dq.PushBack(3);

        std::vector<int> expected = {20, 18, 7, 5, 3};
        std::vector<int> v = {dq[0], dq[1], dq[2], dq[3], dq[4]};
        ASSERT_EQUAL(v, expected);
    }
}

void Test_SubscriptOperator() {
    {
        Deque<int> dq;
        dq.PushFront(3);
        dq.PushFront(6);
        dq.PushFront(11);
        dq.PushFront(14);
        dq.PushFront(18);
        dq.PushFront(20);

        dq.PushBack(2);
        dq.PushBack(1);

        ASSERT_EQUAL(dq[0], 20);
        ASSERT_EQUAL(dq[1], 18);
        ASSERT_EQUAL(dq[2], 14);
        ASSERT_EQUAL(dq[3], 11);
        ASSERT_EQUAL(dq[4], 6);
        ASSERT_EQUAL(dq[5], 3);
        ASSERT_EQUAL(dq[6], 2);
        ASSERT_EQUAL(dq[7], 1);
    }
}

void Test_At() {
    {
        Deque<int> dq;
        dq.PushFront(3);
        dq.PushFront(6);
        dq.PushFront(11);
        dq.PushFront(14);
        dq.PushFront(18);
        dq.PushFront(20);

        dq.PushBack(2);
        dq.PushBack(1);

        ASSERT_EQUAL(dq.At(0), 20);
        ASSERT_EQUAL(dq.At(1), 18);
        ASSERT_EQUAL(dq.At(2), 14);
        ASSERT_EQUAL(dq.At(3), 11);
        ASSERT_EQUAL(dq.At(4), 6);
        ASSERT_EQUAL(dq.At(5), 3);
        ASSERT_EQUAL(dq.At(6), 2);
        ASSERT_EQUAL(dq.At(7), 1);

        try {
            auto temp = dq.At(1000);

            // делаем возможным компиляцию при включенном флаге "-Werror"
            std::cout.setstate(std::ios::failbit);
            std::cout << temp << std::endl;
            std::cout.setstate(std::ios::goodbit);

            // если оказались здесь, то dq.At не выбросил исключение типа "std::out_of_range" при обращении по невалидному индексу
            // => перевыбрасываем исключение, но уже другого типа, так, чтобы его поймал обработчик класса TestRunner
            throw std::runtime_error("Exception \"out_of_range\" was not handled in method Deque<T>::At(size_t index)"s);
        } catch (const std::out_of_range& oor) {
            // PASS
            // если метод At бросил исключение нужного типа, то "гасим" его здесь, в блоке "catch",
            // чтобы оно не было передано в обработчик класса TestRunner
        }
    }
}

void Test_NonConst_At() {
    {
        Deque<int> dq;
        dq.PushFront(7);
        dq.PushFront(18);
        dq.PushFront(20);

        dq.PushBack(5);
        dq.PushBack(3);

        std::vector<int> expected = {20, 18, 5555, 5, 3};

        dq.At(2) = 5555;
        std::vector<int> v = {dq[0], dq[1], dq[2], dq[3], dq[4]};
        ASSERT_EQUAL(v, expected);
    }
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, Test_Size);
    RUN_TEST(tr, Test_Push);
    RUN_TEST(tr, Test_Empty);
    RUN_TEST(tr, Test_SubscriptOperator);
    RUN_TEST(tr, Test_At);
    RUN_TEST(tr, Test_NonConst_At);
}