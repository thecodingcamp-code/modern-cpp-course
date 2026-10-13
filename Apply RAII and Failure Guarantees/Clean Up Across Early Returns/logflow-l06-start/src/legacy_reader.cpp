#include "legacy_reader.h"

#include "file_tracker.h"
#include "log_parser.h"

#include <cstdio>
#include <cstring>

FileReport analyze_legacy(const std::string& path) {
    FileReport report;
    report.path = path;

    std::FILE* file = tracked_open(path.c_str());
    if (file == nullptr) {
        report.failure = "cannot open file";
        return report;
    }

    Counts counts;
    char buffer[512];
    std::size_t line_number = 0;
    while (std::fgets(buffer, sizeof buffer, file) != nullptr) {
        ++line_number;
        std::size_t length = std::strlen(buffer);
        if (length == sizeof buffer - 1 && buffer[length - 1] != '\n') {
            tracked_close(file);
            report.failure = "line " + std::to_string(line_number) + " too long";
            return report;
        }
        while (length > 0 && (buffer[length - 1] == '\n' || buffer[length - 1] == '\r')) {
            buffer[--length] = '\0';
        }
        if (length == 0) {
            continue;
        }

        std::optional<LogRecord> record = parse_line(buffer);
        if (!record) {
            report.failure = "malformed line " + std::to_string(line_number);
            return report;
        }

        switch (record->level) {
            case Level::Info:
                ++counts.info;
                break;
            case Level::Warn:
                ++counts.warn;
                break;
            case Level::Error:
                ++counts.error;
                break;
        }
    }

    tracked_close(file);
    report.counts = counts;
    return report;
}
