#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <memory>
#include <stdexcept>

#include "edge.h"

// Screen dimension constants
constexpr int SCREEN_WIDTH = 1550;
constexpr int SCREEN_HEIGHT = 800;

class Visualization {
public:
    Visualization() = default;
    ~Visualization() {
        // Cleanup is handled by RAII through unique_ptr
    }

    // Delete copy operations
    Visualization(const Visualization&) = delete;
    Visualization& operator=(const Visualization&) = delete;

    // Allow move operations
    Visualization(Visualization&&) = default;
    Visualization& operator=(Visualization&&) = default;

    struct SDLDeleter {
        void operator()(SDL_Window* window) const { SDL_DestroyWindow(window); }
        void operator()(SDL_Renderer* renderer) const { SDL_DestroyRenderer(renderer); }
        void operator()(TTF_Font* font) const { TTF_CloseFont(font); }
        void operator()(SDL_Surface* surface) const { SDL_FreeSurface(surface); }
        void operator()(SDL_Texture* texture) const { SDL_DestroyTexture(texture); }
    };

    using WindowPtr = std::unique_ptr<SDL_Window, SDLDeleter>;
    using RendererPtr = std::unique_ptr<SDL_Renderer, SDLDeleter>;
    using FontPtr = std::unique_ptr<TTF_Font, SDLDeleter>;
    using SurfacePtr = std::unique_ptr<SDL_Surface, SDLDeleter>;
    using TexturePtr = std::unique_ptr<SDL_Texture, SDLDeleter>;

    bool init(WindowPtr& window, RendererPtr& renderer) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        if (TTF_Init() == -1) {
            std::cerr << "TTF could not initialize! TTF Error: " << TTF_GetError() << std::endl;
            return false;
        }

        window.reset(SDL_CreateWindow("Graph Display", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
        
        if (!window) {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
        if (!renderer) {
            std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        return true;
    }

    FontPtr loadFont(const std::string& fontPath, int fontSize) {
        FontPtr font(TTF_OpenFont(fontPath.c_str(), fontSize));
        if (!font) {
            std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        }
        return font;
    }

    void renderText(SDL_Renderer* renderer, TTF_Font* font, 
                   const std::string& text, int x, int y, SDL_Color color) {
        SurfacePtr textSurface(TTF_RenderText_Solid(font, text.c_str(), color));
        if (!textSurface) {
            std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return;
        }

        TexturePtr textTexture(SDL_CreateTextureFromSurface(renderer, textSurface.get()));
        if (!textTexture) {
            std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
            return;
        }

        SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture.get(), nullptr, &renderQuad);
    }
};


