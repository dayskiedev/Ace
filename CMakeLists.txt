# initial build: cmake -S . -B build
# cmake --build build     				  < Debug
# cmake --build build --config Release    < Release (VS build mode)
# then run with ./build/debug/ace.exe

# ensure they have gcc 
# ensure they have git

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

FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git  	# source for imgui we are using
   	GIT_TAG dac07199cfd761113d966eb8ad739254e10df2fe		# release 1.92.7 (stick to release, dont want the latest each time)
)

FetchContent_MakeAvailable(imgui)

# we are basically just downloading raw h and cpp files, and organising them into a lib to compile ith the exec_program(
# we could just put these in target sources but this is cleaner
add_library(imgui 
	${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp

    # SDL3 backend
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp

    # renderer backend
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC SDL3::SDL3)

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

# link SDL3 (SDL3 provides a Cmake target, imgui doesnt)
target_link_libraries(ace PRIVATE SDL3::SDL3 imgui)

#file(COPY roms DESTINATION $<TARGET_FILE_DIR:tgt>)

# this lets use use commands from cmake CLI
add_custom_command(
	TARGET ace
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/Roms
		$<TARGET_FILE_DIR:ace>/Roms
)

# copy the SDL lib to source so those without it should be able to run it?
add_custom_command(
	TARGET ace
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		$<TARGET_FILE:SDL3::SDL3>
		$<TARGET_FILE_DIR:ace>
)

