#include "sock_refs_counter.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <PID> <socket_fd>" << std::endl;
        return 1;
    }

    try {
        pid_t pid = std::stoi(argv[1]);
        int fd = std::stoi(argv[2]);
        SocketReferenceCounter counter(pid, fd);
        std::cout << "Глобальное количество ссылок на сокет: " << counter.count_references() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
