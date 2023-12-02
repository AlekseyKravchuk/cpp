#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

// Класс Worker умеет обрабатывать команды, которые мы подаем в стандартном вводе.
// На команде "stop" работа останавливается
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
        Cv.wait(lock, [&ready = Ready]() { return ready; });

        std::cout << "Command: " << Str << std::endl;

        Ready = false;
    }
}

int main() {
    auto worker = std::make_shared<Worker>();
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
