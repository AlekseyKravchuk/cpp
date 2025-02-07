#include "sock_refs_counter.h"

#include <algorithm>
#include <filesystem>
#include <sys/stat.h>

namespace fs = std::filesystem;
using namespace std;


SocketReferenceCounter::SocketReferenceCounter(pid_t pid, int fd) {
    string fd_path = "/proc/" + to_string(pid) + "/fd/" + to_string(fd);
    struct stat stat_buf{};

    if (stat(fd_path.c_str(), &stat_buf) != 0) {
        throw std::runtime_error("Failed to stat socket descriptor");
    }
    inode = stat_buf.st_ino;
}

int SocketReferenceCounter::count_references() const {
    int count = 0;

    for (const auto& entry: fs::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;
        std::string pid = entry.path().filename().string();
        if (!std::all_of(pid.begin(), pid.end(), ::isdigit)) continue;

        std::string fd_path = "/proc/" + pid + "/fd";
        for (const auto& fd_entry: fs::directory_iterator(fd_path, fs::directory_options::skip_permission_denied)) {
            struct stat stat_buf{};
            if (stat(fd_entry.path().c_str(), &stat_buf) == 0 && stat_buf.st_ino == inode) {
                ++count;
                break; // Учитываем процесс только один раз
            }
        }
    }
    return count;
}

