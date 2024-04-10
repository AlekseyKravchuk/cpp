#include "File.h"

#include <cstdio>
#include <iostream>

using namespace std;

/*
 * Нам нигде не пришлось явно вызывать функцию "fclose", которая закрывает файл (кроме как в деструкторе класса).
 * Наша переменная типа "FILE*" автоматическая, т.е. каким бы способом она не вышла из блока (scope): через
 * "return" или если был было сгенерировано исключение, она всё равно будет освобождена.
 */
int main() {
    try {  // Если файл был открыт НЕуспешно, то сгенерируется исключение, поэтому этот блок нужно обернуть в try-catch
        File f("output.txt");

//        File f2 = f;  // ERROR: free(): double free detected in tcache 2

        f.Write("Hello, world!\n");
        f.Write("This file was opened using RAII idiom.\n");
    } catch (...) {  // ловим исключения любых типов
        cout << "Cannot open file\n" << endl;
    }
}
