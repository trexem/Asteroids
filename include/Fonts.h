#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

class Fonts {
public:
    static inline TTF_Font* Title = nullptr;
    static inline TTF_Font* Subtitle = nullptr;
    static inline TTF_Font* Body = nullptr;

    static void loadFonts() {
        Title = loadFont("data/fonts/consola.ttf", 58);
        Subtitle = loadFont("data/fonts/consola.ttf", 48);
        Body = loadFont("data/fonts/consola.ttf", 24);
    }

    static TTF_Font* loadFont(const char* route, float size) {
        TTF_Font* t_font = TTF_OpenFont(route, size);
        if (t_font == NULL) {
            std::cout << "Failed to load consola font! SDL_ttf Error:" << SDL_GetError() << '\n';
        }
        return t_font;
    }

    static void cleanFonts() {
        if (Title) TTF_CloseFont(Title);
        if (Subtitle) TTF_CloseFont(Subtitle);
        if (Body) TTF_CloseFont(Body);

    }
};
