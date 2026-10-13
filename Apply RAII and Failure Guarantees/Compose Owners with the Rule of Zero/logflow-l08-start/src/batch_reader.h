#pragma once

#include <cstddef>
#include <cstdio>
#include <string>

#include "log_parser.h"

class BatchReader {
public:
    explicit BatchReader(const std::string& path);
    ~BatchReader();
    BatchReader(const BatchReader&) = delete;
    BatchReader& operator=(const BatchReader&) = delete;

    bool is_open() const;
    bool next(LogRecord& record);
    std::size_t rejected_count() const;
    std::size_t rejected_at(std::size_t index) const;

private:
    void remember_rejected(std::size_t line_number);

    std::FILE* file_;
    char* buffer_;
    std::size_t line_number_;
    std::size_t* rejected_;
    std::size_t rejected_count_;
    std::size_t rejected_capacity_;
};
