#include "batch_reader.h"

#include <cstring>
#include <optional>

namespace {
constexpr std::size_t kBufferSize = 512;
}

BatchReader::BatchReader(const std::string& path)
    : file_(std::fopen(path.c_str(), "r")),
      buffer_(new char[kBufferSize]),
      line_number_(0),
      rejected_(nullptr),
      rejected_count_(0),
      rejected_capacity_(0) {}

BatchReader::~BatchReader() {
    delete[] rejected_;
    delete[] buffer_;
    if (file_ != nullptr) {
        std::fclose(file_);
    }
}

bool BatchReader::is_open() const {
    return file_ != nullptr;
}

bool BatchReader::next(LogRecord& record) {
    if (file_ == nullptr) {
        return false;
    }
    while (std::fgets(buffer_, kBufferSize, file_) != nullptr) {
        ++line_number_;
        std::size_t length = std::strlen(buffer_);
        while (length > 0 && (buffer_[length - 1] == '\n' || buffer_[length - 1] == '\r')) {
            buffer_[--length] = '\0';
        }
        if (length == 0) {
            continue;
        }
        std::optional<LogRecord> parsed = parse_line(buffer_);
        if (!parsed) {
            remember_rejected(line_number_);
            continue;
        }
        record = *parsed;
        return true;
    }
    return false;
}

std::size_t BatchReader::rejected_count() const {
    return rejected_count_;
}

std::size_t BatchReader::rejected_at(std::size_t index) const {
    return rejected_[index];
}

void BatchReader::remember_rejected(std::size_t line_number) {
    if (rejected_count_ == rejected_capacity_) {
        std::size_t new_capacity = rejected_capacity_ == 0 ? 4 : rejected_capacity_ * 2;
        std::size_t* grown = new std::size_t[new_capacity];
        for (std::size_t i = 0; i < rejected_count_; ++i) {
            grown[i] = rejected_[i];
        }
        delete[] rejected_;
        rejected_ = grown;
        rejected_capacity_ = new_capacity;
    }
    rejected_[rejected_count_++] = line_number;
}
