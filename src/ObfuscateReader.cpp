#include "ObfuscateReader.h"
#include "EnvReader.h"

#include <fstream>
#include <iostream>
#include <SDL3/SDL_iostream.h>
#include "Log.h"

bool ObfuscatedReader::load(const std::string& path, std::vector<uint8_t>& outBuffer) {
#if defined(__ANDROID__)
    uint8_t KEY = 0xE3;
    std::string HEADER = "TREX";
#else
    if (!EnvReader::load()) {
        std::cerr << "Could not load .env file!" << std::endl;
        return false;
    }
    uint8_t KEY = EnvReader::getByte("KEY");
    std::string HEADER = EnvReader::get("HEADER");
#endif

    SDL_IOStream* stream = SDL_IOFromFile(path.c_str(), "rb");
    if (!stream) {
        std::cerr << "Failed to open obfuscated pack: " << path << std::endl;
        return false;
    }

    char header[4];
    if (SDL_ReadIO(stream, header, sizeof(header)) != sizeof(header)) {
        DEBUG_LOG("Failed to read header from pack file: %s", SDL_GetError());
        SDL_CloseIO(stream);
        return false;
    }

    if (std::string(header, 4) != HEADER) {
        std::cerr << "Invalid asset file header!" << std::endl;
        SDL_CloseIO(stream);
        return false;
    }

    Sint64 size = SDL_GetIOSize(stream);
    if (size <= 4) {
        std::cerr << "Pack file too small or invalid size" << std::endl;
        SDL_CloseIO(stream);
        return false;
    }

    Sint64 payloadSize = size - 4;
    outBuffer.resize(payloadSize);

    Sint64 bytesRead = SDL_ReadIO(stream, outBuffer.data(), payloadSize);
    SDL_CloseIO(stream);

    if (bytesRead != payloadSize) {
        std::cerr << "Failed to read full file data (" << bytesRead << " vs expected " << payloadSize << ")\n";
        return false;
    }

    // XOR decode
    for (size_t i = 0; i < outBuffer.size(); ++i) {
        outBuffer[i] ^= KEY;
    }

    return true;
}
