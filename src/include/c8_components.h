#include <iostream>
#include <stack>

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
uint8_t MEMORY[4096];                   // total virtual memory allocated
uint16_t PROGRAM_COUNTER = 512;         // starting address in decimal (0x200)
uint16_t INDEX_REGISTER = 0;            // points to a location in memory
std::stack<uint16_t> ADDRESS_STACK;     // used to call subroutines/functions
uint8_t DELAY_TIMER = 0;                // decrements at 60hz until 0
uint8_t SOUND_TIMER = 0;                // does the same thing but beeps when not 0

// general purpose registers
uint8_t V0;
uint8_t V1;
uint8_t V2;
uint8_t V3;
uint8_t V4;
uint8_t V5;
uint8_t V6;
uint8_t V7;
uint8_t V8;
uint8_t V9;
uint8_t VA;
uint8_t VB;
uint8_t VC;
uint8_t VD;
uint8_t VE;
uint8_t VF; // also used as a flag register, many instructions set it to 0 or 1 based on some rules

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
uint8_t FONT[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
