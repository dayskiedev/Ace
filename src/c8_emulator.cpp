#include "c8_emulator.h"

bool c8_emulator::Startup(std::string path_to_rom) {
    // define the font we will use
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

    // load font into memory from addr 050 (80)
    for(uint8_t i = 0; i < fontStartAddr; ++i) {
        // offset memory addr by fontstartAddr where we want to begin.
        MEMORY[i + fontStartAddr] = FONT[i];
    }

    // next we attempt to load a rom from the specified path
    std::cout << "Input path: " << path_to_rom << std::endl;
    const unsigned int START_ADR = 0x200;

    std::cout << "Attempting to load ROM...\n";
    char* buffer;
    std::ifstream::pos_type size;
    // open to the end of the file, specifying its a binary file
    std::ifstream rom (path_to_rom,std::ios::binary | std::ios::ate);
    if(rom.is_open()) {
        size = rom.tellg();
        buffer = new char [size];
        rom.seekg(0, std::ios::beg);
        rom.read(buffer, size);
        rom.close();

        std::cout << "ROM is " << size << " bytes\n";
        romSize = size;
        // now that we have the rom loaded into the buffer, we move it to memory
        for(int i = 0; i < size; ++i) {
            MEMORY[START_ADR + i] = buffer[i];
            //std::cout << (int)MEMORY[START_ADR + i] << std::endl;
        }        
        
        delete[] buffer;
    } else {
        std::cout << "Unable to load ROM...\n";
        return false;
    }


    // set PC to inital start point
    PROGRAM_COUNTER = 0x200;

    return true;
}

// IBM OPCODES (OPCODES NEEDED TO RUN IMB LOGO.CH8):

// 00E0 [CLS]: Clear the display set all pixels to off
// 1NNN [JMP NNN]: Set PROGRAM_COUNTER to NNN
// 6XNN [LD, VX, NN]: Load immediate value NN into register VX
// 7XNN [ADD VX, NN]: Add immediate value NN to register VX. Does not effect VF.
// ANNN [LD I, NNN]: Set INDEX_REGISTER equal to NNN
// DXYN [DRW VX, VY, N]: Display N-byte sprite starting at memory location INDEX_REGISTER at 
//                       (VX, VY). Each set bit of xored with what's already drawn. 
//                       VF is set to 1 if a collision occurs. 0 otherwise.



void c8_emulator::Run() {

    // have some sort of stepper function so we can pause/resume a program

    // fetch 
    // i saw some weird trick to combine the bits...
    uint16_t opcode = MEMORY[PROGRAM_COUNTER];
    std::cout << std::hex << "0x" << std::setw(4) << std::setfill('0') << opcode << std::endl;    // decode 
    opcode <<= 8; // shift 8 bits left
    std::cout << "0x" << std::setw(4) << std::setfill('0') << opcode << std::endl;    // decode 
    opcode |= MEMORY[PROGRAM_COUNTER+1];
    std::cout << "0x" << std::setw(4) << std::setfill('0') << opcode << std::endl;    // decode 

    // move 2 spaces in memory (since we take in two bytes for each opcode)
    PROGRAM_COUNTER+=2;

    // execute
}