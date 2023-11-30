counter = [0]

def coinChangeNaiveRecursiveTotalCombinations(denominations, idx, amount):
    global counter[0] += 1
    if(amount == 0):
        return 1
    if(amount < 0):
        return 0
    if(idx == len(denominations)):
        return 0
    return coinChangeNaiveRecursiveTotalCombinations(denominations, idx, amount - denominations[idx]) + coinChangeNaiveRecursiveTotalCombinations(denominations, idx+1, amount)


def recMC(coinValueList, change):
    # counter2[0] += 1
    minCoins = change
    if change in coinValueList:
        return 1
    else:
        for i in [c for c in coinValueList if c <= change]:
            numCoins = 1 + recMC(coinValueList,change-i)
            if numCoins < minCoins:
                minCoins = numCoins
    return minCoins


def main():
    denominations = [1, 5, 10, 25]




if __name__ == "__main__":
    main()
