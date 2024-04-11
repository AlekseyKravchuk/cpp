#include <cstdio>

using namespace std;

int main() {
    // открываем файл и получаем его дескриптор
    char fname[] = "output.txt";
    FILE* fp = fopen(fname, "w");  // FILE* - указатель на внутреннюю структуру "_IO_FILE"
    if (fp != nullptr) {
//        fprintf(fp, "Hello, world!\n");
        fputs("Hello, world!\n", fp);
        fputs("This file is written with fputs\n", fp);
        // ... some job
        fclose(fp);  // главное не забыть открытый файл в конце блока (не забыть вернуть ресурс операционной системе)
    } else {
        printf("Error while opening the file \"%s\"\n", fname);
    }
}
