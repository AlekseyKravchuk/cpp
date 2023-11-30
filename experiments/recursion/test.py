class Counter(object):
    def __init__(self, fun):
        self._fun = fun
        self.counter = 0

    def __call__(self, *args, **kwargs):
        self.counter += 1
        return self._fun(*args, **kwargs)


def recur(n):
    print('recur', n)
    if n > 0:
        return recur(n-1)
    return 0


recur = Counter(recur)

recur(5)

print('# of times recur has been called =', recur.counter)
