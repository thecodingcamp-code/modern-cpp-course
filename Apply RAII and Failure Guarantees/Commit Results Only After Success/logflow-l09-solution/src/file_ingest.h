#pragma once

#include "run_totals.h"

#include <string>
#include <vector>

struct BatchResult {
    RunTotals totals;
    std::vector<std::string> failures;
};

void ingest_file(const std::string& path, RunTotals& totals);
BatchResult ingest_all(const std::vector<std::string>& paths);
