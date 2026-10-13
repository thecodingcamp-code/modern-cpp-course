#include "file_tracker.h"
#include "legacy_reader.h"

#include <iostream>
#include <string>

namespace {

int failures = 0;

void check(bool passed, const std::string& name) {
    if (passed) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cout << "[FAIL] " << name << "\n";
        ++failures;
    }
}

void run_case(const std::string& name, const std::string& path, const std::string& expected_failure,
              std::size_t expected_records) {
    int before = open_handle_count();
    FileReport report = analyze_legacy(path);
    int leaked = open_handle_count() - before;

    check(report.failure == expected_failure, name + ": result");
    check(report.counts.total() == expected_records, name + ": record count");
    if (leaked == 0) {
        check(true, name + ": all handles closed");
    } else {
        check(false, name + ": " + std::to_string(leaked) + " handle(s) still open");
    }
}

}  // namespace

int main() {
    run_case("clean file", "data/leaky/clean.log", "", 6);
    run_case("missing file", "data/leaky/missing.log", "cannot open file", 0);
    run_case("malformed line", "data/leaky/malformed-line.log", "malformed line 3", 0);
    run_case("overlong line", "data/leaky/overlong-line.log", "line 2 too long", 0);

    if (failures == 0) {
        std::cout << "all early-exit cleanup tests passed\n";
    } else {
        std::cout << failures << " check(s) failed\n";
    }
    return failures;
}
