//
// Created by stati on 3/23/2026.
//

#ifndef EMULATOR_PLATFORM_H
#define EMULATOR_PLATFORM_H
#include <string>
#include <SDL3/SDL.h>

namespace Emulator {
    class Platform {
    public:
        Platform(const std::string &title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
        ~Platform();

        void Update(void const *buffer, int pitch) const;
        bool ProcessInput(uint8_t *keys);

    private:
        SDL_Window *window{};
        SDL_Renderer *renderer{};
        SDL_Texture *texture{};
    };
}


#endif //EMULATOR_PLATFORM_H
