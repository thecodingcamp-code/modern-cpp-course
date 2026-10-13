#include "report.h"

#include <algorithm>
#include <filesystem>
#include <iomanip>

namespace {

std::string display_name(const FileReport& report) {
    return std::filesystem::path(report.path).filename().string();
}

void print_row(std::ostream& out, const std::string& name, std::size_t records,
               const Counts& counts) {
    out << std::left << std::setw(24) << name << std::right << std::setw(9) << records
        << std::setw(7) << counts.info << std::setw(7) << counts.warn << std::setw(7)
        << counts.error << "\n";
}

}  // namespace

void print_report(std::ostream& out, std::vector<FileReport> reports) {
    std::sort(reports.begin(), reports.end(), [](const FileReport& a, const FileReport& b) {
        return display_name(a) < display_name(b);
    });

    out << "LogFlow report\n";
    out << "==============\n";
    out << std::left << std::setw(24) << "file" << std::right << std::setw(9) << "records"
        << std::setw(7) << "info" << std::setw(7) << "warn" << std::setw(7) << "error" << "\n";

    Counts total;
    std::size_t processed = 0;
    for (const FileReport& report : reports) {
        if (!report.ok()) {
            continue;
        }
        print_row(out, display_name(report), report.counts.total(), report.counts);
        total.info += report.counts.info;
        total.warn += report.counts.warn;
        total.error += report.counts.error;
        ++processed;
    }
    print_row(out, "total", total.total(), total);

    out << "\nfiles processed: " << processed << "\n";
    out << "files rejected: " << reports.size() - processed << "\n";
    for (const FileReport& report : reports) {
        if (!report.ok()) {
            out << "  " << display_name(report) << ": " << report.failure << "\n";
        }
    }
}
