#include "renderdevice.h"

RenderDevice* RenderDevice_create() {
    RenderDevice* rd = (RenderDevice*)malloc(sizeof(RenderDevice));
    return rd;
}

bool RenderDevice_init(RenderDevice* rd, int windowWidth, int windowHeight) {
    rd->window = SDL_CreateWindow("Hello SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (rd->window == NULL) {
        printf( "SDL Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    rd->renderer = SDL_CreateRenderer(rd->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rd->renderer) {
        printf( "SDL Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    // Black
    SDL_SetRenderDrawColor(rd->renderer, 0x00, 0x00, 0x00, 0xFF);
}

void RenderDevice_destroy(RenderDevice* rd) {
    SDL_DestroyRenderer(rd->renderer);
    SDL_DestroyWindow(rd->window);
    rd->renderer = NULL;
    rd->window = NULL;
    delete rd;
}
