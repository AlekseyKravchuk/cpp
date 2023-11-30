def isGreedySuitable(val, denominations):
    denominations.sort(reverse=True)
    usedDenominations = [0 for _ in range(len(denominations))]
    current = 0
    # print(f"len(denominations) = {len(denominations)}")
    print()
    while val > 0 and current < len(denominations):
        if denominations[current] <= val:
            val -= denominations[current]
            usedDenominations[current] += 1
        else:
            current += 1
        print(val, usedDenominations)
    return val


def main():
    val = 100
    denominations = [30, 1, 20, 50]
    current = 0

    if isGreedySuitable(val, denominations) != 0:
        print("Greedy algorithm can't by applied for solving this problem!")
    else:
        print("The algorithm is suitable!")


# if __name__ == "__main__":
#     main()
