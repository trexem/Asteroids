#pragma once

#include <string>
#include <vector>
#include <cstdint>

class ObfuscatedReader {
public:
    static bool load(const std::string& path, std::vector<uint8_t>& outBuffer);
};
