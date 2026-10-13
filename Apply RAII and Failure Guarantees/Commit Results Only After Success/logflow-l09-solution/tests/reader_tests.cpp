#include "batch_reader.h"

#include <iostream>
#include <string>
#include <type_traits>

static_assert(std::is_move_constructible_v<BatchReader>, "BatchReader should work without hand-written special members");

namespace {

int failures = 0;

void check(bool passed, const std::string& name) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << "\n";
    if (!passed) {
        ++failures;
    }
}

}  // namespace

int main() {
    {
        BatchReader reader("data/rules/noisy-collector.log");
        LogRecord record;
        std::size_t records = 0;
        while (reader.next(record)) {
            ++records;
        }
        check(reader.is_open(), "noisy file: opened");
        check(records == 7, "noisy file: 7 valid records");
        check(reader.rejected_count() == 5, "noisy file: 5 rejected lines");
        check(reader.rejected_at(0) == 2 && reader.rejected_at(4) == 11, "noisy file: rejected line numbers");
    }
    {
        BatchReader reader("data/rules/missing.log");
        LogRecord record;
        check(!reader.is_open(), "missing file: not open");
        check(!reader.next(record), "missing file: nothing to read");
    }

    if (failures == 0) {
        std::cout << "all reader tests passed\n";
    }
    return failures;
}
