#include "h.h"
#include  "X.h"/*  */

int main() {
    // если в "h.h" переменная объявлена без ключевого слова "inline", то linker выдаст ошибку
    // "multiple definition of `i';"
    f1();
    X xObj;

    return 0;
}
