#include "analyzer.h"
#include "report.h"

#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: logflow <log-file> [<log-file> ...]\n";
        return 2;
    }

    std::vector<FileReport> reports;
    for (int i = 1; i < argc; ++i) {
        reports.push_back(analyze_file(argv[i]));
    }

    print_report(std::cout, reports);

    bool any_rejected = std::any_of(reports.begin(), reports.end(),
                                    [](const FileReport& report) { return !report.ok(); });
    return any_rejected ? 1 : 0;
}
