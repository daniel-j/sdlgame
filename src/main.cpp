#include <stdio.h>
#include <iostream>

#include "renderdevice.h"
#include "fpscounter.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

RenderDevice* rd = NULL;

// The image we will load and show on the screen
SDL_Texture* gTexture = NULL;

TTF_Font* gFont = NULL;

bool init() {
    // Initialize SDL
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    rd = new RenderDevice();

    rd->init(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    gFont = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans.ttf", 24);
    if (gFont == NULL) {
        fprintf(stderr, "SDL2_ttf error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

SDL_Texture* loadTexture(const char* path) {
    // The final optimized image
    SDL_Texture* texture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if ( loadedSurface == NULL ) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    // Convert surface to screen format
    texture = SDL_CreateTextureFromSurface(rd->renderer, loadedSurface);
    if ( texture == NULL ) {
        printf("Unable to create texture from image %s! SDL Error: %s\n", path, SDL_GetError());
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return texture;
}

void close() {
    // Destroy texture
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;
    rd->destroy();
    rd = NULL;

    TTF_CloseFont(gFont);
    gFont = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void renderText(TTF_Font* font, int x, int y,
                const char* text, SDL_Color* color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, *color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rd->renderer, surface);
    SDL_Rect pos = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(rd->renderer, texture, NULL, &pos);
    SDL_DestroyTexture(texture);
}

int main(int argc, char *argv[]) {
    // Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
        close();
        return 1;
    }

    gTexture = loadTexture("../background.png");
    if (!gTexture) {
        printf("Failed to load media!\n");
        close();
        return 1;
    }

    bool quit = false;
    bool minimized = false;
    SDL_Color textColor = { 255, 255, 255 };

    SDL_Event e;
    // init fps counter
    fpsinit();
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (e.window.event) {
                        case SDL_WINDOWEVENT_MINIMIZED:
                            minimized = true;
                            break;
                        case SDL_WINDOWEVENT_RESTORED:
                            minimized = false;
                            break;
                    }
                    break;
            }
        }

        // update logic

        // update fps counter
        float framespersecond = fpsupdate();

        // Clear screen
        SDL_RenderClear(rd->renderer);

        // Draw texture
        SDL_RenderCopy(rd->renderer, gTexture, NULL, NULL);

        // Draw fps counter
        renderText(gFont, 0, 0, std::to_string(static_cast<int>(framespersecond)).c_str(),
                   &textColor);

        SDL_RenderPresent(rd->renderer);

        // To prevent 100% cpu usage
        SDL_Delay(1);
    }

    // Free resources and close SDL
    close();
    return 0;
}
