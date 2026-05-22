# ACE (Another Chip8 Emulator)
My version of a Chip8 emulator (Also using it to practice CMake)

## Install/Run
1. Clone the project to your computer
2. Ensure you have a C++17 complient compiler installed 
3. Ensure you have CMake installed
4. Inside the root directory of the project run:
```cmake
cmake -S . -B build
```
**!The makefile will install SDL3, SDL_Mixer and ImGUi into the project directory even if you already have them downloaded!**

5. Once the configuration has finished, run:
```cmake
cmake --build build
```
6. Once the program has been built, simply run it, for windows its:
```
.\build\Debug\ace.exe 
```
* Or you can click on the Ace.exe file itself

You should be greeted with the following:

<center><img src="images/example.png" alt="drawing" width="500"/></>