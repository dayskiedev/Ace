# initial build: cmake -S . -B build
# then to actually build: cmake --build build
# then run with ./build/debug/ace.exe

# ensure they have gcc 
# ensure they have git
# check sdl3 dll is installed?
# need to copy dll to path

cmake_minimum_required(VERSION 3.24)

# ensure user has at least this version of c++
set (CMAKE_CXX_STANDARD 17)

project(ace)
include(FetchContent)
add_executable(ace)

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git	# source for SDL we are using
   	GIT_TAG 5848e584a1b606de26e3dbd1c7e4ecbc34f807a6		# release 3.4.4 (stick to release, dont want the latest each time)
   	OVERRIDE_FIND_PACKAGE  								    # ensure this fetchcontent has first been called
)

# check if the user has SDL3 installed, if not we want to include it.
FetchContent_MakeAvailable(SDL3)

# get project files
target_sources(ace
	PRIVATE
    	src/main.cpp
		src/c8_emulator.cpp
		src/c8_utils.cpp
	PUBLIC 
		FILE_SET HEADERS
		BASE_DIRS
			src/include
		FILES
			src/include/c8_emulator.h
			src/include/c8_font.h
			src/include/c8_utils.h
)


# link SDL3
target_link_libraries(ace PRIVATE SDL3::SDL3)