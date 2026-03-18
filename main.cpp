#include "spdlog/spdlog.h"
#include "SDL3/SDL.h"

int main() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        spdlog::error("Failed to initialize the SDL2 library");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL3 Window", 800, 600, 0);

    if(!window)
    {
        spdlog::error("Failed to create window");
        return -1;
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        spdlog::error("Failed to get the surface from the window");
        return -1;
    }

    SDL_UpdateWindowSurface(window);

    SDL_Delay(5000);
    return EXIT_SUCCESS;
}
