#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "testHeader.h"

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


// CHIP8 Specifc
// memory is 4096 bytes
// when we talk about loading/reading memory at 0x200, its local to THIS MEMORY
// we DONT load into ACTUAL memory, we create an array of VIRTUAL memory and
// go to it using decimal conversion

// so program counter is an int starting a 0x200 which is actually 512

// int8 is 1 byte, meaning it can store only 0->255 values
// but this is for EACH int, we have 4096 of them
// we want it to be 1 byte because each instruction is only 1 byte, and we need to read 2 at a time
// this stops any accidental skips
uint8_t MEMORY[4096];           // total virtual memory allocated
uint16_t PROGRAM_COUNTER = 512; // starting address in decimal (0x200)

// font is written using bits, where 1 is a black pixel and 0 white. Here is an example of 0:
// 0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
// 4 pixels wide and 5 pixels tall
// 11110000        1111
// 10010000        1001
// 10010000        1001
// 10010000        1001
// 11110000        1111
// as you can we we end up with 0 in binary, and reducing it to be 4x5 we see it cleaer

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
    MEMORY[PROGRAM_COUNTER] = 21;

    // uint8_t is treated as character type not number, so it tries to find ascii 21
    std::cout << (int)MEMORY[PROGRAM_COUNTER] << " " << PROGRAM_COUNTER << std::endl;


    int exitCode { 0 };
    testHeader t;
    std::cout << "Launching... with test num " << t.testNum << "\n";

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