#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// change build structure/initial setup, run cmake -S . -B build
// to actually build, run cmake --build build
// then ./debug/ace.exe

// if SDL3 is properly installed, this should work

// constexpr evaluates at compile rather than runtime
// brace init for type safety (?)
constexpr int ScreenWidth { 500 };
constexpr int ScreenHeight { 500 }; 

SDL_Window* gWindow { nullptr };
SDL_Surface* gScreenSurface { nullptr };

bool init()
{
    //Initialization flag
    bool success{ true };

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) == false )
    {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        if( gWindow = SDL_CreateWindow( "SDL3 Tutorial: Hello SDL3", ScreenWidth, ScreenHeight, 0 ); gWindow == nullptr )
        {
            SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}


void close()
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

    std::cout << "Launching...\n";

    if(init() == false) {
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
            while( SDL_PollEvent( &e ) == true )
            {
                //If event is quit type
                if( e.type == SDL_EVENT_QUIT )
                {
                    //End the main loop
                    quit = true;
                }
            }

            //Fill the surface white
            SDL_FillSurfaceRect( gScreenSurface, nullptr, SDL_MapSurfaceRGB( gScreenSurface, 0xFF, 0xFF, 0xFF ) );


            //Update the surface
            SDL_UpdateWindowSurface( gWindow );
        }
    }


    close();

    return exitCode;
   
}