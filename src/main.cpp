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
constexpr int RELATIVE_SCREEN_WIDTH { 32 };
constexpr int RELATIVE_SCREEN_HEIGHT { 64 }; 
std::string PROGRAM_NAME { "ACE" };

int RESOLUTION_SCALE { 12 };

SDL_Window* gWindow { nullptr };
SDL_Surface* gScreenSurface { nullptr };
SDL_Renderer* gRenderer { nullptr };

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

	SDL_SetRenderDrawColor(gRenderer, 255, 175, 222, 0xFF);
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

            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            SDL_RenderClear(gRenderer);
            SDL_RenderPresent(gRenderer);
        }
    }


    Close();

    return exitCode;
   
}