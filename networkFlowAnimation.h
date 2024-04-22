#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

#include "edge.h"

using namespace std;
// Screen dimension constants
const int SCREEN_WIDTH = 1550;
const int SCREEN_HEIGHT = 800;


class Visualization {
public:
    bool init(SDL_Window** window, SDL_Renderer** renderer) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        if (TTF_Init() == -1) {
            std::cerr << "TTF could not initialize! TTF Error: " << TTF_GetError() << std::endl;
            return false;
        }

        *window = SDL_CreateWindow("Graph Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!*window) {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
        if (!*renderer) {
            std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        return true;
    }

    // Load font
    TTF_Font* loadFont(const std::string& fontPath, int fontSize) {
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!font) {
            std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        }
        return font;
    }

    // Render text
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!textSurface) {
            std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        }
        else {
            SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
        }
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }


};


