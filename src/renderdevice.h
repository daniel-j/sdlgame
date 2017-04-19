#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <SDL2/SDL.h>

class RenderDevice {
 public:
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    RenderDevice();
    ~RenderDevice();
    bool init(int windowWidth, int windowHeight);
    void destroy();
};

#endif  // RENDERDEVICE_H
