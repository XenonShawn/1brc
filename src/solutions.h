#pragma once

#include <string>

namespace Solutions {
    void naive(std::string filename);
    void better(std::string filename);
    void memory_map(const char* filename);
    void multithreaded(const char* filename);
}