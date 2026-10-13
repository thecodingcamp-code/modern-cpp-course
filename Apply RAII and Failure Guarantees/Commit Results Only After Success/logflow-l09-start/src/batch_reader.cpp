#include "batch_reader.h"

#include <optional>

BatchReader::BatchReader(const std::string& path) : in_(path) {}

bool BatchReader::is_open() const {
    return in_.is_open();
}

bool BatchReader::next(LogRecord& record) {
    while (std::getline(in_, line_)) {
        ++line_number_;
        if (!line_.empty() && line_.back() == '\r') {
            line_.pop_back();
        }
        if (line_.empty()) {
            continue;
        }
        std::optional<LogRecord> parsed = parse_line(line_);
        if (!parsed) {
            rejected_.push_back(line_number_);
            continue;
        }
        record = *parsed;
        return true;
    }
    return false;
}

std::size_t BatchReader::rejected_count() const {
    return rejected_.size();
}

std::size_t BatchReader::rejected_at(std::size_t index) const {
    return rejected_[index];
}
