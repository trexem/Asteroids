#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>

class EnvReader {
public:
    static bool load(const std::string& filePath = ".env");
    static std::string get(const std::string& key);
    static uint8_t getByte(const std::string& key);

private:
    static inline std::unordered_map<std::string, std::string> values;
};
