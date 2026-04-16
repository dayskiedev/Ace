# initial build: cmake -S . -B build
# then to actually build: cmake --build build
# then run with ./build/debug/ace.exe
cmake_minimum_required(VERSION 3.24)

# ensure user has at least this version of c++
set (CMAKE_CXX_STANDARD 17)

project(ace)
include(FetchContent)
add_executable(ace)

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
   	GIT_TAG origin/main
   	OVERRIDE_FIND_PACKAGE  # ensure this fetchcontent has first been called
)

# check if the user has SDL3 installed, if not we want to include it.
FetchContent_MakeAvailable(SDL3)

# get project files
target_sources(ace
   PRIVATE
      src/main.cpp
)


# link SDL3
target_link_libraries(ace PRIVATE SDL3::SDL3)