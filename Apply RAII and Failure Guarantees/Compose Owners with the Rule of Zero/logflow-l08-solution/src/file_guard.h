#pragma once

#include "file_tracker.h"

#include <cstdio>

class FileGuard {
public:
    explicit FileGuard(const char* path) : file_(tracked_open(path)) {}

    ~FileGuard() { tracked_close(file_); }

    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    std::FILE* get() const { return file_; }

private:
    std::FILE* file_;
};
