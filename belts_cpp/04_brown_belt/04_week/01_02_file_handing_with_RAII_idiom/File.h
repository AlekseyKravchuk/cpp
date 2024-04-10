#pragma once

#include <cstdio>
#include <string>
#include <stdexcept>

class File {
  public:
    File(const std::string& filename);
    File(const File&) = delete;
    void operator=(const File&) = delete;
    ~File();

    const std::string& GetFileName() const;
    void Write(const std::string& line);

  private:
    FILE* _fp;
    const std::string& _filename;
};
