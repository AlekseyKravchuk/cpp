#!/usr/bin/env python3


class Test:
    """Is used to store key to search and array to search in"""

    def __init__(self):
        """Constructor"""
        self.key = 0    # key to search
        self.nums = []  # array to search a key within


def binSearch(arr, key):
    low = 0
    high = len(arr)

    while low <= high:
        mid = low + (high - low)//2
        if key == arr[mid]:
            return mid

        if key < arr[mid]:
            high = mid - 1
        else:
            low = mid + 1

    return -1


def main():
    # input() in Python3 always return a string object
    tests_num = int(input())  # the number of tests
    print(f"Program will be executed for {tests_num} tests ")
    tests = []

    if tests_num <= 0:
        print("Wrong number of tests to be checked!")
        return -1

    for i in range(tests_num):
        test = Test()
        nums_size, test.key = [int(a) for a in input().split()]
        print(f"{i}: nums_size = {nums_size}, test.key = {test.key}")

        test.nums = [int(x) for x in input().split()]
        if len(test.nums) != nums_size:
            print("Incorrect number of elements in array!")
            return -1
        tests.append(test)

    for i in range(len(tests)):
        print(tests[i].nums)

    index = 0
    for i in tests:
        print(f"iteration {index}: {binSearch(i.nums, i.key)}; \
        nums = {i.nums},  key = {i.key}")
        index += 1


if __name__ == "__main__":
    main()
