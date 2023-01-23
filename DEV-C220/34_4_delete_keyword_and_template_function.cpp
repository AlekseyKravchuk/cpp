// глобальная шаблонная функция, тип "T" может быть любым
template <typename T>
void pf(T* ptr) {
}

// можно поставить запрет на инстанцирование конкретных специализаций
// в частности, на специализацию, которая принимает "const char*" накладываем запрет
template <>
void pf(const char* ptr) = delete;

int main() {
    int n = 1;
    pf(&n);        // OK
    // pf("abc");  // ERROR

    return 0;
}
