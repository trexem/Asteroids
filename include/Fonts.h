#pragma once

#include <SDL3/SDL_log.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

#include "PackReader.h"
#include "utils.hpp"

class Fonts {
public:
    static inline TTF_Font* Title = nullptr;
    static inline TTF_Font* Subtitle = nullptr;
    static inline TTF_Font* Body = nullptr;

    static inline std::unordered_map<std::string, std::vector<uint8_t>> fontBuffers;

    static void loadFonts() {
        Title = loadFont("fonts/consola.ttf", 58, "Title");
        Subtitle = loadFont("fonts/consola.ttf", 48, "Subtitle");
        Body = loadFont("fonts/consola.ttf", 24, "Body");
    }

    static TTF_Font* loadFont(const std::string& route, float size, const std::string& id) {
        auto buffer = PackReader::instance().readFile(normalizePath(route));
        if (buffer.empty()) {
            SDL_Log("Failed to read font file from pack: %s", route.c_str());
            return nullptr;
        }

        fontBuffers[id] = std::move(buffer);

        SDL_IOStream* stream = SDL_IOFromMem(fontBuffers[id].data(), fontBuffers[id].size());
        if (!stream) {
            SDL_Log("Failed to create IO stream for font: %s", route.c_str());
            return nullptr;
        }

        TTF_Font* font = TTF_OpenFontIO(stream, 1, size);
        if (font == NULL) {
            SDL_Log("Failed to load consola font! SDL_ttf Error: %s", SDL_GetError());
        }
        return font;
    }

    static void cleanFonts() {
        if (Title) { TTF_CloseFont(Title); Title = nullptr; }
        if (Subtitle) { TTF_CloseFont(Subtitle); Subtitle = nullptr; }
        if (Body) { TTF_CloseFont(Body); Body = nullptr; }
    }
};
