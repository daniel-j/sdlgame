#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <SDL2/SDL.h>

struct RenderDevice {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
};

RenderDevice* RenderDevice_create();
bool RenderDevice_init(RenderDevice* rd, int windowWidth, int windowHeight);
void RenderDevice_destroy(RenderDevice* rd);

#endif // RENDERDEVICE_H
