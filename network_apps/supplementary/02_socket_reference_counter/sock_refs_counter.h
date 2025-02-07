#pragma once

#include <sys/socket.h>
#include <iostream>

class SocketReferenceCounter {
  public:
    explicit SocketReferenceCounter(pid_t pid, int fd);
    int count_references() const;

  private:
    ino_t inode;
};
