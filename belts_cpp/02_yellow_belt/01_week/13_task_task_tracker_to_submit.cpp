#include <array>  // std::array
#include <iostream>
#include <map>
#include <stdexcept>
#include <tuple>

using namespace std::literals;

TaskStatus NextStatus(TaskStatus status) {
    if (status != TaskStatus::DONE) {
        return static_cast<TaskStatus>((static_cast<int>(status) + 1));
    } else {
        throw std::runtime_error("Current status is \"TaskStatus::DONE\", operation \"NextStatus\" not applicable"s);
    }
}

// overloading of prefix increment operator for "TaskStatus"
TaskStatus& operator++(TaskStatus& status) {
    return status = NextStatus(status);
}

class TeamTasks {
   public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
        return _names2tinfo.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const std::string& person) {
        ++_names2tinfo[person][TaskStatus::NEW];
    }

    // Обновить статусы по данному количеству задач конкретного разработчика
    std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {
        TasksInfo updated_tasks;
        TasksInfo untouched_tasks;
        TasksInfo& tasks = _names2tinfo[person];
        int done_tasks_count = tasks[TaskStatus::DONE];

        for (auto status = TaskStatus::NEW; status != TaskStatus::DONE; ++status) {
            if (tasks.count(status) == 0) {
                continue;
            }

            // "task_count == 0" => больше задач, подлежащих выполнению, нет и "status != TaskStatus::DONE"
            // => все задачи данного статуса должны быть учтены как необработанные, т.е. в "untouched_tasks"
            if (task_count == 0) {
                untouched_tasks[status] = tasks.at(status);
                continue;
            }

            int count = tasks.at(status);
            auto next_status = NextStatus(status);

            // проверяем, не осталось ли необработанных задач в текущем статусе
            // если "task_count < count", то в текущем статусе останутся необработанные задачи
            if (task_count < count) {
                updated_tasks[next_status] = task_count;
                untouched_tasks[status] = count - task_count;
                task_count = 0;
            } else {
                // если "task_count >= count", необработанных задач текущего статуса не останется
                updated_tasks[next_status] = count;
                task_count -= count;
            }

            // ведем отдельный учет DONE-задач (выполненных задач)
            if (next_status == TaskStatus::DONE) {
                if (updated_tasks.count(next_status)) {
                    done_tasks_count += updated_tasks[next_status];
                }
            }
        }

        // обнуляем словарь "tasks"
        tasks.clear();

        // добавляем DONE-задачи
        if (done_tasks_count) {
            tasks[TaskStatus::DONE] = done_tasks_count;
        }

        // восстанавливаем актуальные значения "_names2tinfo" в соответствии с "updated_tasks" и "untouched_tasks"
        for (auto status = TaskStatus::NEW; status != TaskStatus::DONE; ++status) {
            if (untouched_tasks.count(status)) {
                tasks[status] += untouched_tasks[status];
            }

            if (updated_tasks.count(status)) {
                tasks[status] += updated_tasks[status];
            }
        }

        return {updated_tasks, untouched_tasks};
    }

   private:
    std::map<std::string, TasksInfo> _names2tinfo;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    std::cout << tasks_info[TaskStatus::NEW] << " new tasks"
              << ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"
              << ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested"
              << ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << std::endl;
}
