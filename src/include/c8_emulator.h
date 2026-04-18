#ifndef C8_COMPONENTS
#define C8_COMPONENTS

#include <iostream>
#include <stack>
#include <fstream>
#include <iomanip>

// right now we get duplicate linker errors because this file is duplicated accross 
// multiple instances
// we need to move the definitions into a cpp file
// c8_emulator.cpp? and define them there

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

// the _t ensures we have the same size across multiple systems

class c8_emulator {
    public:
    bool Startup(std::string path_to_rom);

    // should limit to 60hz to match timer coundown?
    void Run();

    // read-only veiw of all memory
    const uint8_t* GetMemory() { return MEMORY; }
    const int GetRomSize() { return romSize; }
    const int GetStartAddr() { return 0x200; }

    private:
    uint8_t MEMORY[4096];                    // total virtual memory allocated
    uint16_t PROGRAM_COUNTER = {0};          // starting address in decimal (0x200)
    uint16_t INDEX_REGISTER = {0};           // points to a location in memory
    std::stack<uint16_t> ADDRESS_STACK;      // used to call subroutines/functions
    uint8_t DELAY_TIMER {0};                 // decrements at 60hz until 0
    uint8_t SOUND_TIMER {0};                 // does the same thing but beeps when not 0

    uint8_t REGISTERS[16]{};                 // general purpose registers

    uint8_t fontStartAddr{0x50};

    // Debug info
    int romSize{};
    // grabbing char = beginning address * size of char (5 bytes) + start

    // we manually write the font here, then load it into memory on startup...
    
};


#endif