#include "log_parser.h"

#include <sstream>

namespace {

std::optional<Level> parse_level(std::string_view text) {
    if (text == "INFO") {
        return Level::Info;
    }
    if (text == "WARN") {
        return Level::Warn;
    }
    if (text == "ERROR") {
        return Level::Error;
    }
    return std::nullopt;
}

}  // namespace

std::optional<LogRecord> parse_line(std::string_view line) {
    std::istringstream in{std::string(line)};
    std::string timestamp;
    std::string level_text;
    std::string service;
    if (!(in >> timestamp >> level_text >> service)) {
        return std::nullopt;
    }

    std::optional<Level> level = parse_level(level_text);
    if (!level) {
        return std::nullopt;
    }

    std::string message;
    std::getline(in >> std::ws, message);
    if (message.empty()) {
        return std::nullopt;
    }

    return LogRecord{timestamp, *level, service, message};
}
