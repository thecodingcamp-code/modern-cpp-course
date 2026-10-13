#pragma once

#include <cstdio>

std::FILE* tracked_open(const char* path);
void tracked_close(std::FILE* file);
int open_handle_count();
