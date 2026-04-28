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

        std::cout << "ROM is " << size << " bytes (" << size/2 << " instructions)\n";
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
    PROGRAM_COUNTER = START_ADR;

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



void c8_emulator::Cycle() {

    // have some sort of stepper function so we can pause/resume a program


    // fetch
    uint16_t opcode = MEMORY[PROGRAM_COUNTER];

    // we start by assigning opcode the first byte
    // its stored in a 16bit variable, so we grab the memory value at 512 
    // which in ibm is: 00 or in binary 00000000
    // note that when starting out, our opcode is empty: 000000000000
    // this is so we can fit two bytes in, which is why we shift by 8 bits
    // although in this case it will still be 0
    // opcode: 0000000000000000
    // then we increment the program counter by 1 reading the next piece of memory:
    // e0 which is 11100000
    // note that if we just set the opcode to this, it would overwrite our previous value
    // and come out like 0000000011100000 (which i mean matches, but again not the point)
    // so instead we perfrom an OR bitwise operation, which turns any matching 1's from the opcode
    // into 1's that match the resulting value in e0. this leaves us with a 16 bit opcode of:
    // 00000000000011100000 or 00e0 which we can do with as we pleases

    // decode
    opcode <<= 8;
    opcode |= MEMORY[PROGRAM_COUNTER+1];
    //std::cout << std::hex << "0x" << std::uppercase << std::setw(4) << std::setfill('0') << opcode << std::endl;    // decode
    // 1111000000000000 to get first 4 bits for instruction

    // uint8 treated as char when printing, no real reason to force an 8 bit
    // 0xF000 == 1111000000000000
    // >> 12 makes it 0000000000001111

    // fN = first nibbles
    uint16_t fN = ((opcode & 0xF000) >> 12);
    std::cout << std::hex << fN << std::endl;
    //execute       
    PROGRAM_COUNTER+=2; 
}