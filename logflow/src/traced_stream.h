#pragma once

#include <fstream>
#include <iostream>
#include <string>

class TracedStream {
public:
    explicit TracedStream(const std::string& path) : path_(path), stream_(path) {
        std::cerr << "[trace] acquire " << path_ << "\n";
    }

    ~TracedStream() {
        std::cerr << "[trace] release " << path_ << "\n";
    }

    std::ifstream& stream() { return stream_; }

private:
    std::string path_;
    std::ifstream stream_;
};
