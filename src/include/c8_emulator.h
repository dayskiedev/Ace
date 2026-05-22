#ifndef C8_COMPONENTS
#define C8_COMPONENTS

#include <iostream>
#include <stack>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cstdlib>

#include "c8_font.h"

#include "SDL3_mixer/SDL_mixer.h"

#define PIXEL_ON 0xFFFFFFFF
#define PIXEL_OFF 0x00000000

class c8_emulator {
    public:
    bool Startup(std::string path_to_rom);

    void Cycle();
    void Tick();

    // set register value from input 
    void SetInputValue(uint16_t input) { INPUT_VALUE = input; }

    // read-only veiw of all memory
    const uint8_t* GetMemory() { return MEMORY; }
    const uint8_t GetMemoryValue(int addr) { return MEMORY[addr]; }

    const int GetRomSize() { return romSize; }
    const int GetStartAddr() { return START_ADR; }
    
    const int GetPC() { return PROGRAM_COUNTER; }
    const int GetIR() { return INDEX_REGISTER; }
    const int GetRV(int RX) { return REGISTERS[RX]; }
    const std::stack<uint16_t> GetAS() { return ADDRESS_STACK; } // should be a reference somehow...

    // on = 0xFFFFFFFF 
    // off = 0x00000000
    uint32_t VIDEO [64 * 32]{};              // screen resolution (use % to handle wrapping )
    // 1d array 

    private:
    uint8_t MEMORY[4096];                    // total virtual memory allocated
    uint16_t START_ADR = 0x200;
    uint16_t PROGRAM_COUNTER = {0};          // starting address in decimal (0x200)
    uint16_t INDEX_REGISTER = {0};           // points to a location in memory
    std::stack<uint16_t> ADDRESS_STACK;      // used to call subroutines/functions
    uint8_t DELAY_TIMER {0};                 // decrements at 60hz until 0
    uint8_t SOUND_TIMER {0};                 // does the same thing but beeps when not 0
    uint8_t REGISTERS[16]{};                 // general purpose registers

    uint8_t INPUT_VALUE {99};                // default to 99 so out of range, if value is anything but 99 we know we have an input?

    MIX_Mixer *mixer;
    MIX_Audio *beep;

    // Toggles
    bool _USE_OLD_MEMORY_STORE_LOAD = true; // In older chip8 implementations, when storing or loading memory the IR would be incremented by X+1, however newer models dont do this
    bool _USE_OLD_JUMP_WITH_OFFSET = true;   // older versions have this instruction set the PC to NNN + V0, but modern versions change this to PC = XNN + reg[vx]
    bool _USE_OLD_BIT_SHIFT = false;         // older versions first set VX to the value of VY before shifting

    // Debug info
    int romSize{};
    int _RAND_SEED = 42;   // Makes it easy to recreate random events for debugging 
    //bool _OUTPUT_OPCODES = false;
    
};


#endif