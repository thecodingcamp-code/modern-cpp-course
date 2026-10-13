#include "analyzer.h"

#include "log_parser.h"
#include "traced_stream.h"

#include <iostream>

FileReport analyze_file(const std::string& path) {
    FileReport report;
    report.path = path;

    TracedStream source(path);
    std::ifstream& in = source.stream();
    if (!in) {
        report.failure = "cannot open file";
        return report;
    }

    Counts counts;
    std::string line;
    std::size_t line_number = 0;
    while (std::getline(in, line)) {
        ++line_number;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }

        std::optional<LogRecord> record = parse_line(line);
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

    report.counts = counts;
    std::cerr << "[trace] analyze_file reached its return\n";
    return report;
}
