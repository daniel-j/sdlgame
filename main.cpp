#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

// The image we will load and show on the screen
SDL_Texture* gTexture = NULL;

TTF_Font* gFont = NULL;

// FPS counter from:
// http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement

#define FRAME_VALUES 10

// An array to store frame times:
Uint32 frametimes[FRAME_VALUES];

// Last calculated SDL_GetTicks
Uint32 frametimelast;

// total frames rendered
Uint32 framecount;

// the value you want
float framespersecond;

// This function gets called once on startup.
void fpsinit() {
	// Set all frame times to 0ms.
	memset(frametimes, 0, sizeof(frametimes));
	framecount = 0;
	framespersecond = 0;
	frametimelast = SDL_GetTicks();
}

void fpsupdate() {
	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	Uint32 i;

	// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
	// This value rotates back to 0 after it hits FRAME_VALUES.
	frametimesindex = framecount % FRAME_VALUES;

	// store the current time
	getticks = SDL_GetTicks();

	// save the frame time value
	frametimes[frametimesindex] = getticks - frametimelast;

	// save the last frame time for the next fpsthink
	frametimelast = getticks;

	// increment the frame count
	framecount++;

	// Work out the current framerate

	// The code below could be moved into another function if you don't need the value every frame.

	// I've included a test to see if the whole array has been written to or not. This will stop
	// strange values on the first few (FRAME_VALUES) frames.
	if (framecount < FRAME_VALUES) {
		count = framecount;
	} else {
		count = FRAME_VALUES;
	}

	// add up all the values and divide to get the average frame time.
	framespersecond = 0;
	for (i = 0; i < count; i++) {
		framespersecond += frametimes[i];
	}
	framespersecond /= count;

	// now to make it an actual frames per second value...
	framespersecond = 1000.f / framespersecond;
}

bool init() {
	// Initialize SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}

	// Create window
	gWindow = SDL_CreateWindow( "Hello SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if ( gWindow == NULL ) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}

	gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if ( !gRenderer ) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if ( !( IMG_Init( imgFlags ) & imgFlags ) ) {
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return false;
	}

	if (TTF_Init() == -1) {
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}

	gFont = TTF_OpenFont( "/usr/share/fonts/TTF/DejaVuSans.ttf", 24 );
	if (gFont == NULL) {
		fprintf(stderr, "SDL2_ttf error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

SDL_Texture* loadTexture( std::string path ) {
	// The final optimized image
	SDL_Texture* texture = NULL;

	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if ( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		return NULL;
	}

	// Convert surface to screen format
	texture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
	if ( texture == NULL ) {
		printf( "Unable to create texture from image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}

	// Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );

	return texture;
}

void close() {
	// Destroy texture
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
	// Destroy renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	// Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	TTF_CloseFont(gFont);
	gFont = NULL;

	// Quit SDL subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void renderText(TTF_Font* font, int x, int y, const char* text, SDL_Color* color) {
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, *color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_Rect pos = {x, y, surface->w, surface->h};
	SDL_FreeSurface(surface);
	SDL_RenderCopy(gRenderer, texture, NULL, &pos);
	SDL_DestroyTexture(texture);
}

int main(int argc, char *argv[]) {

	// Start up SDL and create window
	if (!init()) {
		printf( "Failed to initialize!\n" );
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
	SDL_Surface *fpsCounterSurface = NULL;
	SDL_Event e;
	// init fps counter
	fpsinit();
	while ( !quit ) {
		// Handle events on queue
		while (SDL_PollEvent( &e ) != 0) {
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
		fpsupdate();

		// Clear screen
		SDL_RenderClear(gRenderer);

		// Draw texture
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		// Draw fps counter
		renderText(gFont, 0, 0, to_string(int(framespersecond)).c_str(), &textColor);

		SDL_RenderPresent(gRenderer);

		// To prevent 100% cpu usage
		SDL_Delay(1);
	}

	// Free resources and close SDL
	close();
	return 0;
}
