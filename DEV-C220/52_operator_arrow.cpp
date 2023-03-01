/*
Специфика оператора ->.
Функциональная форма выглядит: ptrA.operator->()->SomeMethod();
При желании одну -> можно и в три "развернуть".
Только для этого матрешка из оберток немножко по-другому должна выглядеть:
*/
class A {
    int m_a;

   protected:
    A(int a) { m_a = a; }

   public:
    void func(){};
    friend class ptr;
};

class ptr {
    A* m_pA;

   public:
    ptr(int a = 0) { m_pA = new A(a); }
    ~ptr(void) { delete m_pA; }
    A* operator->() { return m_pA; }
};

class ptr1 {
    ptr m_pP;

   public:
    ptr1(int a = 0) : m_pP(a) {}
    ptr& operator->() { return m_pP; }
};

class ptr2 {
    ptr1 m_pP1;

   public:
    ptr2(int a = 0) : m_pP1(a) {}
    ptr1& operator->() { return m_pP1; }
};

int main() {
    ptr2 my2(3);
    my2->func();

    return 0;
}
