#ifndef C8_COMPONENTS
#define C8_COMPONENTS

#include <iostream>
#include <stack>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <algorithm>

#include "c8_font.h"

#define PIXEL_ON 0xFFFFFFFF
#define PIXEL_OFF 0x00000000

class c8_emulator {
    public:
    bool Startup(std::string path_to_rom);

    // should limit to 60hz to match timer coundown?
    void Cycle();

    // read-only veiw of all memory
    const uint8_t* GetMemory() { return MEMORY; }
    const int GetRomSize() { return romSize; }
    const int GetStartAddr() { return START_ADR; }

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
    


    // Debug info
    int romSize{};
    bool increment = true; // hacky solution so specific instructions can stop the PC from incremeneting
    
};


#endif