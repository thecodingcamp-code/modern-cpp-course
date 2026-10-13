#include "file_ingest.h"

#include <iostream>
#include <string>

namespace {

int failures = 0;

void check(bool passed, const std::string& name) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << "\n";
    if (!passed) {
        ++failures;
    }
}

bool same(const RunTotals& a, const RunTotals& b) {
    return a.counts.info == b.counts.info && a.counts.warn == b.counts.warn &&
           a.counts.error == b.counts.error && a.files_accepted == b.files_accepted;
}

std::string describe(const RunTotals& t) {
    return std::to_string(t.counts.total()) + " records, " + std::to_string(t.files_accepted) + " files";
}

void check_rejected(const std::string& name, const std::string& path, const std::string& expected_error) {
    RunTotals totals;
    ingest_file("data/commit/orders-ok.log", totals);
    RunTotals before = totals;

    std::string error;
    try {
        ingest_file(path, totals);
    } catch (const std::exception& e) {
        error = e.what();
    }

    check(error == expected_error, name + ": rejected with the right message");
    if (same(totals, before)) {
        check(true, name + ": totals unchanged");
    } else {
        std::cout << "       before: " << describe(before) << "\n       after:  " << describe(totals) << "\n";
        check(false, name + ": totals unchanged");
    }
}

}  // namespace

int main() {
    RunTotals totals;
    ingest_file("data/commit/orders-ok.log", totals);
    check(totals.counts.total() == 6 && totals.files_accepted == 1, "good file adds all 6 records");

    check_rejected("corrupt file", "data/commit/orders-corrupt.log", "malformed line 5");
    check_rejected("missing file", "data/commit/missing.log", "cannot open file");

    BatchResult batch = ingest_all({"data/commit/orders-ok.log", "data/commit/orders-corrupt.log",
                                    "data/commit/refunds-ok.log"});
    check(batch.totals.counts.total() == 11 && batch.totals.files_accepted == 2, "batch counts only accepted files");
    check(batch.failures.size() == 1, "batch reports one rejected file");

    if (failures == 0) {
        std::cout << "all commit tests passed\n";
    }
    return failures;
}
