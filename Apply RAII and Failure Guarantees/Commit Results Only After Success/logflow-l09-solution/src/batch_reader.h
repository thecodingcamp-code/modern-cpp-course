#pragma once

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

#include "log_parser.h"

class BatchReader {
public:
    explicit BatchReader(const std::string& path);

    bool is_open() const;
    bool next(LogRecord& record);
    std::size_t rejected_count() const;
    std::size_t rejected_at(std::size_t index) const;

private:
    std::ifstream in_;
    std::string line_;
    std::size_t line_number_ = 0;
    std::vector<std::size_t> rejected_;
};
