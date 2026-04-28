#include <iostream>
#include <stack>
#include <bitset>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "c8_emulator.h"
#include "c8_utils.h"

// ACE Chip8 Emulator 
// Brody Watson 2026
// Ref: https://tobiasvl.github.io/blog/write-a-chip-8-emulator/

// change build structure/initial setup, run cmake -S . -B build
// to actually build, run cmake --build build
// then ./debug/ace.exe

// if SDL3 is properly installed, this should work

// constexpr evaluates at compile rather than runtime
// brace init for type safety (?)
constexpr int SCREEN_WIDTH { 32 };
constexpr int SCREEN_HEIGHT { 64 }; 
std::string PROGRAM_NAME { "Ace" };

int SCREEN_SCALE { 10 };

SDL_Window* gWindow { nullptr };
SDL_Renderer* gRenderer { nullptr };

// Check specific memory location for font and print
// it to see if output is valid

bool Init() {
	//initialze sdl
	if (SDL_Init(SDL_INIT_VIDEO) == false) {
		std::cout << "SDL could not be initialised!" << std::endl;
		return false;
	}
	std::cout << "SDL initialised" << std::endl;

    // Create window
    gWindow = SDL_CreateWindow(PROGRAM_NAME.c_str(), SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (gWindow == NULL) {
        std::cout << "Window could not be created!" << std::endl;
        return false;
    }
    std::cout << "Window created" << std::endl;

    // Create Renderer for window
    gRenderer = SDL_CreateRenderer(gWindow, NULL);
    if (gRenderer == NULL) {
        std::cout << "Renderer could not be created: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "Renderer created" << std::endl;
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderScale(gRenderer, SCREEN_SCALE, SCREEN_SCALE);
    SDL_SetWindowSize(gWindow, SCREEN_WIDTH * SCREEN_SCALE,  SCREEN_HEIGHT * SCREEN_SCALE);


	return true;
}
void Close()
{
    std::cout << "Exiting...\n";
    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    //Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* args[]){
    // if(argc <= 1) {
    //     std::cout << "Error, no ROM specified (include the path to ROM in launch) \nClosing...\n";
    //     return -1;
    // }

    //check fonts loaded correctly 
    //c8_utils utils;
    //utils.CheckFont(fontStartAddr, false);

    // lives only in these brackets, which is fine
    c8_emulator emulator;
    c8_utils utils;

    std::string path_to_rom = "roms/IBM Logo.ch8";
    if(!emulator.Startup(path_to_rom)) {
        std::cout << "Error loading emulator, unable to launch.";
        return -1;
    }
    int cycles = 4;

    bool cycle = false;

    for(int i = 0; i < cycles; ++i) {
        emulator.Cycle();
    }
    //utils.CheckFont(emulator.GetMemory(), 80, false);
    
    // 0x50 -> fonts loaded from here
    // 0x200 -> rom loaded here

    // IBM end addr  -> 0x284
    // Pong end addr -> 0x308
    // note: actual values are stored as uint8_t but printed as hex
    //utils.CheckMemory(emulator.GetMemory(), emulator.GetStartAddr(), emulator.GetRomSize() + emulator.GetStartAddr());

    int exitCode { 0 };
    std::cout << "Launching... \n";

    if(Init() == false) {
        SDL_Log("Unable to init!\n");
        exitCode = 1;
    } 

    else {
        bool quit { false }; 

        SDL_Event e;
        SDL_zero( e );

         //The main loop
        while( quit == false )
        {   
            //Get event data
            SDL_PollEvent( &e );
            if( e.type == SDL_EVENT_QUIT ) { quit = true; }

            // call updates here
            // pass through renderer to update screen?

            if(e.type == SDL_EVENT_KEY_DOWN) {
                if(e.key.key == SDLK_C) {
                    if(cycle = true) {
                        emulator.Cycle();
                        cycle = false;
                    }
                }
            } else if(e.type == SDL_EVENT_KEY_UP) {
                if(e.key.key == SDLK_C) {
                    cycle = true;
                }
            }

            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderPresent(gRenderer);
            SDL_RenderClear(gRenderer);
        }
    }


    Close();

    return exitCode;
   
}