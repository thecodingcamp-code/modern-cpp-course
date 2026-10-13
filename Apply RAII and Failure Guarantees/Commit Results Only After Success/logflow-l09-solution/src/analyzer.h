#pragma once

#include <cstddef>
#include <string>

struct Counts {
    std::size_t info = 0;
    std::size_t warn = 0;
    std::size_t error = 0;

    std::size_t total() const { return info + warn + error; }
};

struct FileReport {
    std::string path;
    Counts counts;
    std::string failure;

    bool ok() const { return failure.empty(); }
};

FileReport analyze_file(const std::string& path);
