#pragma once

#include <optional>
#include <string>
#include <string_view>

enum class Level { Info, Warn, Error };

struct LogRecord {
    std::string timestamp;
    Level level;
    std::string service;
    std::string message;
};

std::optional<LogRecord> parse_line(std::string_view line);
