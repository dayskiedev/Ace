#ifndef C8_COMPONENTS
#define C8_COMPONENTS

#include <iostream>
#include <stack>
#include <fstream>

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

    // read-only veiw of all memory
    const uint8_t* GetMemory() { return MEMORY; }

    private:
    uint8_t MEMORY[4096];                   // total virtual memory allocated
    uint16_t PROGRAM_COUNTER = {512};       // starting address in decimal (0x200)
    uint16_t INDEX_REGISTER = {0};          // points to a location in memory
    std::stack<uint16_t> ADDRESS_STACK;     // used to call subroutines/functions
    uint8_t DELAY_TIMER {0};                // decrements at 60hz until 0
    uint8_t SOUND_TIMER {0};                // does the same thing but beeps when not 0

    // general purpose registers
    uint8_t REGISTERS[16]{};

    // font is written using bits, where 1 is a black pixel and 0 white. Here is an example of 0:
    // 0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    // 4 pixels wide and 5 pixels tall
    // 11110000        1111
    // 10010000        1001
    // 10010000        1001
    // 10010000        1001
    // 11110000        1111
    // as you can we we end up with 0 in binary, and reducing it to be 4x5 we see it cleaer
    // binary to hex, break it into 4 bit chunks 1111 0000 then multiply by power of position
    // 2^3 +

    // think of each as a chunk of raw memory
    // starting at 050 and ending at 09f
    // also the size of our array!
    uint8_t fontStartAddr = 80;
    // grabbing char = beginning address * size of char (5 bytes) + start

    // we manually write the font here, then load it into memory on startup...
    
};


#endif