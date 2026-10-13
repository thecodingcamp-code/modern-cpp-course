#include "file_tracker.h"

namespace {
int open_handles = 0;
}

std::FILE* tracked_open(const char* path) {
    std::FILE* file = std::fopen(path, "r");
    if (file != nullptr) {
        ++open_handles;
    }
    return file;
}

void tracked_close(std::FILE* file) {
    if (file != nullptr) {
        std::fclose(file);
        --open_handles;
    }
}

int open_handle_count() {
    return open_handles;
}
