#include "renderdevice.h"

RenderDevice::RenderDevice() {
}
RenderDevice::~RenderDevice() {
    destroy();
}

bool RenderDevice::init(int windowWidth, int windowHeight) {
    window = SDL_CreateWindow(
        "Hello SDL2",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowWidth, windowHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("SDL Window could not be created! SDL_Error: %s\n",
               SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("SDL Renderer could not be created! SDL_Error: %s\n",
               SDL_GetError());
        return false;
    }
    // Black
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
}

void RenderDevice::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;
}
