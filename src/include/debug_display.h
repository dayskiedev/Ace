#ifndef DEBUG_DISPLAY_H
#define DEBUG_DISPLAY_H

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include <string>
#include <SDL3/SDL.h>

#include "c8_utils.h"
#include "c8_emulator.h"

class debug_display {
    public:
        bool Init(SDL_Window* _window, SDL_Renderer* _renderer);
        void Update(SDL_Event e, c8_utils& utils, c8_emulator& emulator, bool& paused, std::string& romPath);
        void Render(SDL_Renderer* renderer);
        void Close();
    private:
    char rom[128] = "roms/romname.ch8";

};

#endif