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
constexpr int SCREEN_WIDTH { 64 };
constexpr int SCREEN_HEIGHT { 32 }; 
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

    // Create window (suggess using create window and renderer to avoid flicker?)
    gWindow = SDL_CreateWindow(PROGRAM_NAME.c_str(), SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_VULKAN);
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


// we dont want to just set the register whenever
// we want to wait until we are requesting an input
// we only need to set the hexideceimal value

void CheckForInput(SDL_Event& e, c8_emulator& c8) {
    if(e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.key) {
        case SDLK_1:
            // set hex value to 1
            c8.SetInputValue(0x1);
            break;
        case SDLK_2:
            c8.SetInputValue(0x2);
            break;
        case SDLK_3:
            c8.SetInputValue(0x3);
            break;
        case SDLK_4:
            c8.SetInputValue(0xC);
            break;
        case SDLK_Q:
            c8.SetInputValue(0x4);
            break;
        case SDLK_W:
            c8.SetInputValue(0x5);
            break;
        case SDLK_E:
            c8.SetInputValue(0x6);
            break;
        case SDLK_R:
            c8.SetInputValue(0xD);
            break;
        case SDLK_A:
            c8.SetInputValue(0x7);
            break;
        case SDLK_S:
            c8.SetInputValue(0x8);
            break;
        case SDLK_D:
            c8.SetInputValue(0x9);
            break;
        case SDLK_F:
            c8.SetInputValue(0xE);
            break;
        case SDLK_Z:
            c8.SetInputValue(0xA);
            break;
        case SDLK_X:
            c8.SetInputValue(0x0);
            break;
        case SDLK_C:
            c8.SetInputValue(0xB);
            break;
        case SDLK_V:
            c8.SetInputValue(0xF);
            break;
        default:
            break;
        }
    }
    else if(e.type == SDL_EVENT_KEY_UP) {
        c8.SetInputValue(99);
    }

    // when a key is not down, set hexvalue to -99 or something out of range
}
// tick seperate function for timers? can keep timing seperate to cycle and allows them to run when a cycle is paused for input

int main(int argc, char* args[]){
    // if(argc <= 1) {
    //     std::cout << "Error, no ROM specified (include the path to ROM in launch) \nClosing...\n";
    //     return -1;
    // }

    // setup SDL
    if(Init() == false) {
        SDL_Log("Unable to init!\n");
        return -1;
    } 

    c8_emulator emulator;
    c8_utils utils;

    // no idea what SDL_PIXELFORMAT_RGBA8888 does...
    // SDL_TEXTUREACCESS_STREAMING = texture changes constantly (it does)
    // when screen width is doubled you gget 2 copies of the texture, when screen height is doubled it crashes..
    SDL_Texture* videoTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetTextureScaleMode(videoTexture, SDL_SCALEMODE_PIXELART);
    // look into thiss
    int video_pitch = sizeof(emulator.VIDEO[0]) * SCREEN_WIDTH;

    std::string rom = "test_opcode";

    std::string path_to_rom = "roms/" + rom + ".ch8";
    if(!emulator.Startup(path_to_rom)) {
        std::cout << "Error loading emulator, unable to launch.";
        return -1;
    }

    bool cycle = false;
    int exitCode { 0 };
    std::cout << "Launching... \n";

    bool quit { false }; 
    SDL_Event e;
    SDL_zero( e );

        //The main loop
    while( quit == false )
    {   
        //Get event data
        SDL_PollEvent( &e );
        if( e.type == SDL_EVENT_QUIT ) { quit = true; }

        // check for input
        CheckForInput(e, emulator);

        // tick clocks

        // run a cycle of emulator
        emulator.Cycle();

        // if(e.type == SDL_EVENT_KEY_DOWN) {
        //     if(e.key.key == SDLK_C) {
        //         emulator.Cycle();
        //     } 
        //     else if (e.key.key == SDLK_X) {
        //         utils.CheckMemory(emulator.GetMemory(), 0x200, 0x284);
        //     }
        // }
        SDL_RenderClear(gRenderer);
        //SDL_SetRenderDrawColor(gRenderer,0,0,0,0);
        // copy texture to a rect and render that so its seperate from screen width/height?
        SDL_UpdateTexture(videoTexture, nullptr, emulator.VIDEO, video_pitch);
        SDL_RenderTexture(gRenderer, videoTexture, nullptr, nullptr);

        SDL_RenderPresent(gRenderer);
    }

    Close();

    return exitCode;
   
}