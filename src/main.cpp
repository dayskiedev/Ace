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
constexpr int RELATIVE_SCREEN_WIDTH { 320 };
constexpr int RELATIVE_SCREEN_HEIGHT { 640 }; 
std::string PROGRAM_NAME { "ACE" };

int RESOLUTION_SCALE { 1 };

SDL_Window* gWindow { nullptr };
SDL_Surface* gScreenSurface { nullptr };
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
    gWindow = SDL_CreateWindow(PROGRAM_NAME.c_str(), RELATIVE_SCREEN_WIDTH, RELATIVE_SCREEN_HEIGHT, 0);
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

    SDL_SetRenderScale(gRenderer, RESOLUTION_SCALE, RESOLUTION_SCALE);
    SDL_SetWindowSize(gWindow, RELATIVE_SCREEN_WIDTH * RESOLUTION_SCALE,  RELATIVE_SCREEN_HEIGHT * RESOLUTION_SCALE);


	return true;
}
void Close()
{
    std::cout << "Exiting...\n";
    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gScreenSurface = nullptr;

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
    emulator.Startup(path_to_rom);

    //utils.CheckFont(emulator.GetMemory(), 80, false);

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

            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderPresent(gRenderer);
            SDL_RenderClear(gRenderer);
        }
    }


    Close();

    return exitCode;
   
}