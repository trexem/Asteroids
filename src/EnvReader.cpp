#include "EnvReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool EnvReader::load(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in) {
        std::cerr << "[EnvReader] Failed to open " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(in, line)) {
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string key = line.substr(0, eq);
            std::string value = line.substr(eq + 1);
            values[key] = value;
        }
    }
    return true;
}

std::string EnvReader::get(const std::string& key) {
    auto it = values.find(key);
    return it != values.end() ? it->second : "";
}

uint8_t EnvReader::getByte(const std::string& key) {
    std::string val = get(key);
    if (val.starts_with("0x") || val.starts_with("0X")) {
        return static_cast<uint8_t>(std::stoul(val, nullptr, 16));
    }
    return static_cast<uint8_t>(std::stoi(val));
}
