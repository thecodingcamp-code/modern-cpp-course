#include "file_ingest.h"

#include "log_parser.h"

#include <fstream>
#include <optional>
#include <stdexcept>

void ingest_file(const std::string& path, RunTotals& totals) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("cannot open file");
    }

    Counts local;
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
            throw std::runtime_error("malformed line " + std::to_string(line_number));
        }

        switch (record->level) {
            case Level::Info:
                ++local.info;
                break;
            case Level::Warn:
                ++local.warn;
                break;
            case Level::Error:
                ++local.error;
                break;
        }
    }

    totals.counts.info += local.info;
    totals.counts.warn += local.warn;
    totals.counts.error += local.error;
    ++totals.files_accepted;
}

BatchResult ingest_all(const std::vector<std::string>& paths) {
    BatchResult result;
    for (const std::string& path : paths) {
        try {
            ingest_file(path, result.totals);
        } catch (const std::exception& e) {
            result.failures.push_back(path + ": " + e.what());
        }
    }
    return result;
}
