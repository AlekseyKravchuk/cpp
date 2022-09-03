#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

int main() {
    TaskStatus ts = TaskStatus::IN_PROGRESS;
    switch (ts) {
        case TaskStatus::NEW:
            cout << "status NEW" << endl;
            break;
        case TaskStatus::IN_PROGRESS:
            cout << "status IN_PROGRESS" << endl;
            break;

        default:
            break;
    }
    cout << static_cast<int>(ts) << endl;

    return 0;
}
