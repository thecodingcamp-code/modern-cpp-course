#pragma once

#include "analyzer.h"

#include <ostream>
#include <vector>

void print_report(std::ostream& out, std::vector<FileReport> reports);
