#!/usr/bin/python3


def sort_characters(string):
    order = [0] * len(string)
    count = {
        '$': 0,
        "A": 0,
        'C': 0,
        'G': 0,
        'T': 0
    }

    for char in string:
        count[char] += 1

    elements = ['$', 'A', 'C', 'G', 'T']

    for i in range(1, 5):
        count[elements[i]] += count[elements[i - 1]]

    for j in range(len(string) - 1, -1, -1):
        c = string[j]
        count[c] -= 1
        order[count[c]] = j

    return order


def compute_char_classes(s, order):
    char_class = [0] * len(s)

    for i in range(1, len(s)):
        if s[order[i]] == s[order[i - 1]]:
            char_class[order[i]] = char_class[order[i - 1]]
        else:
            char_class[order[i]] = char_class[order[i - 1]] + 1

    return char_class


def sort_doubled(s, L, old_order, old_class):
    count = [0] * len(s)
    new_order = [0] * len(s)

    for i in range(len(s)):
        count[old_class[i]] += 1

    for i in range(1, len(s)):
        count[i] += count[i - 1]

    for j in range(len(s) - 1, -1, -1):
        start = (old_order[j] - L + len(s)) % len(s)
        cl = old_class[start]
        count[cl] -= 1
        new_order[count[cl]] = start

    return new_order


def update_classes(new_order, old_class, L):
    n = len(new_order)
    new_class = [0] * n

    for i in range(1, n):
        cur = new_order[i]
        mid = (cur + L) % n
        prev = new_order[i - 1]
        mid_prev = (prev + L) % n

        if old_class[cur] == old_class[prev] and old_class[mid] == old_class[mid_prev]:
            new_class[cur] = new_class[prev]
        else:
            new_class[cur] = new_class[prev] + 1

    return new_class


def build_suffix_array(s):
    order = sort_characters(s)
    char_class = compute_char_classes(s, order)
    L = 1

    while L < len(s):
        order = sort_doubled(s, L, order, char_class)
        char_class = update_classes(order, char_class, L)
        L = 2 * L

    return order[1:]


def pattern_matching_with_suffix_array(t, p, sa):
    min_ = 0
    max_ = len(t)

    while min_ < max_:
        mid = (min_ + max_) // 2
        suffix = sa[mid]
        i_ = 0

        while i_ < len(p) and suffix + i_ < len(t):
            if p[i_] > t[suffix + i_]:
                min_ = mid + 1
                break
            elif p[i_] < t[suffix + i_]:
                max_ = mid
                break

            i_ += 1

            if i_ == len(p):
                max_ = mid
            elif suffix + i_ == len(t):
                min_ = mid + 1

    start = min_
    max_ = len(t)

    while min_ < max_:
        mid = (min_ + max_) // 2
        suffix = sa[mid]
        i_ = 0

        while i_ < len(p) and suffix + i_ < len(t):
            if p[i_] < t[suffix + i_]:
                max_ = mid
                break

            i_ += 1

            if i_ == len(p) and i_ <= len(t) - suffix:
                min_ = mid + 1

    end = max_ - 1

    return start, end


if __name__ == '__main__':
    text = input()
    n_patterns = int(input())
    patterns = list(input().split())
    suffix_array = build_suffix_array(text + '$')
    result = [0] * len(text)

    for pattern in patterns:
        s, e = pattern_matching_with_suffix_array(text, pattern, suffix_array)

        if s <= e:
            for i in range(s, e + 1):
                pos = suffix_array[i]

                if result[pos] == 0:
                    print(pos, end=' ')

                result[pos] += 1
