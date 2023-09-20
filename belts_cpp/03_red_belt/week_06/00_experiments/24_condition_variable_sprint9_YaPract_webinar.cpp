#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

// Класс "Worker" умеет обрабатывать команды, которые мы подаем через стандартный ввод (на команде "stop" - останов).
// Считывание команд из входного потока осуществляется в одном потоке, а полезная работа - в другом фоновом потоке

class Worker {
  public:
    void Process(std::string str, bool stop);
    void Work();

  private:
    std::string Str;
    std::mutex Mutex;
    std::condition_variable Cv;
    bool Ready = false;
    bool Stop = false;
};

void Worker::Process(std::string str, bool stop) {
    {
        std::lock_guard guard(Mutex);
        Ready = true;
        Stop = stop;
        Str = std::move(str);
    }

    Cv.notify_one();
}

void Worker::Work() {
    while (!Stop) {
        std::unique_lock lock(Mutex);

        // ожидание закончится, как только произойдет вызов "std::condition_variable::notify_one()",
        // т.е. сразу после того, как отработает критическая секция в методе "Worker::Process"
        Cv.wait(lock, [&ready = Ready]() { return ready; });

        std::cout << "Command: " << Str << std::endl;

        Ready = false;
    }
}

int main() {
    auto worker = std::make_shared<Worker>();

    // запускаем поток, который запускает метод "Work", который выводит команду на консоль,
    // как только она появилась (была считана из std::cin) 
    std::thread thread([worker]() {
        worker->Work();
    });

    bool stop = false;
    while (!stop) {
        std::string str;
        std::cin >> str;

        stop = str == "stop";
        worker->Process(std::move(str), stop);
    }

    thread.join();
    return 0;
}
