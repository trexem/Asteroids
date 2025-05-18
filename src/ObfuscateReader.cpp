#include "ObfuscateReader.h"
#include "EnvReader.h"

#include <fstream>
#include <iostream>

bool ObfuscatedReader::load(const std::string& path, std::vector<uint8_t>& outBuffer) {
    if (!EnvReader::load()) {
        std::cerr << "Could not load .env file!" << std::endl;
        return false;
    }
    uint8_t KEY = EnvReader::getByte("KEY");
    std::string HEADER = EnvReader::get("HEADER");

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open obfuscated pack: " << path << std::endl;
        return false;
    }

    char header[4];
    in.read(header, 4);
    if (std::string(header, 4) != HEADER) {
        std::cerr << "Invalid asset file header!" << std::endl;
        std::cerr << "Read Header: " << header << std::endl;
        std::cerr << "Expected Header: " << HEADER << std::endl;
        return false;
    }

    std::vector<uint8_t> encodedData((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    outBuffer.resize(encodedData.size());

    for (size_t i = 0; i < encodedData.size(); ++i) {
        outBuffer[i] = encodedData[i] ^ KEY;
    }

    return true;
}
