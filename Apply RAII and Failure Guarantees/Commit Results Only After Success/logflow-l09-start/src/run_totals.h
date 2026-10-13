#pragma once

#include "analyzer.h"

#include <cstddef>

struct RunTotals {
    Counts counts;
    std::size_t files_accepted = 0;
};
