#include "c8_emulator.h"

bool c8_emulator::Startup(std::string path_to_rom) {
    // load font into memory from addr 050 (80)
    for(uint8_t i = 0; i < fontStartAddr; ++i) {
        // offset memory addr by fontstartAddr value to be where we want to begin.
        MEMORY[i + fontStartAddr] = FONT[i];
    }

    // next we attempt to load a rom from the specified path
    std::cout << "Input path: " << path_to_rom << std::endl;

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

    std::cout << "Loaded ROM!\n";
    // set PC to inital start point
    PROGRAM_COUNTER = START_ADR;

    return true;
}

void c8_emulator::Cycle() {
    if(!increment) { increment = true; }
    

    // [fetch]
    uint16_t opcode = MEMORY[PROGRAM_COUNTER];

    if(PROGRAM_COUNTER > START_ADR + romSize) {
        std::cout << "Outside ROM Memory, terminate program.\n";
        return;
    }
    
    // [decode]
    opcode <<= 8;
    opcode |= MEMORY[PROGRAM_COUNTER+1];
    // n1 = first nibbles
    uint16_t n1 = ((opcode & 0xF000) >> 12);
    //std::cout << std::hex << firstNibble << std::endl;
    uint16_t n2 = ((opcode & 0x0F00) >> 8);
    uint16_t n3 = ((opcode & 0x00F0) >> 4);
    uint16_t n4 = (opcode & 0x000F);

    // could extract from opcode directly
    // there is uint8_t but it messes with printing, including a couple extra bytes for these wont hurt too much
    uint16_t N = n4;
    uint16_t NN = (n3 << 4) | N;
    uint16_t NNN = (n2 << 8) | NN;

    //std::cout << n1 << "|" << n2 << "|" << n3 << "|" << n4 << std::endl;
    std::cout << "Raw opcode: " << std::hex << "0x" << std::uppercase << std::setw(4) << std::setfill('0') << opcode << std::dec << " | ";   
    
    // [execute]
    switch (n1) {
    case 0x0:
        switch (opcode) {
        case 0x00E0:
            std::cout << "Clear screen\n";
            std::fill(VIDEO, VIDEO+((64*32)/2), PIXEL_OFF);
            break;
        default:
            std::cout << "Unknown 0 type nibble...\n";
            break;
        }
        break;

    case 0x1:
        // we should not be incrementing the pc counter after this jump
        std::cout << "Jump to: " << std::hex << NNN <<  std::dec << std::endl;
        PROGRAM_COUNTER = NNN;
         // we dont want to increment the counter after jumping, 
            // note: for the ibm program, this indicates the end of the program, we get into an inf loop
        increment = false;
        break;
    case 0x3:
        std::cout << "Skip 1 instruction if the value in V" << std::hex << n2 << " is equal to " << NN << std::dec << std::endl;
        break;
    case 0x6:
        std::cout << std::hex << "Set register V" << n2 << " to " << std::hex << NN << std::dec << "\n";
        REGISTERS[n2] = NN;
        break;
    case 0x7:
        std::cout << "Add value " << std::hex << NN << std::dec << " to register V" << n2 << "\n";
        REGISTERS[n2] += NN;
        break;
    case 0xA:
        std::cout  << "Set index register I to " << std::hex << NNN << std::dec << "\n";
        INDEX_REGISTER = NNN;
        break;
    case 0xC:
        // this also may indicate an end of file.
        std::cout << "Generate a random number, AND it with NN and place in register VX\n";
        break;
    case 0xD:
        std::cout << "Display/Draw\n";

        

        break;
    default:
        std::cout << "Unknown Nibble... \n";
        break;
    }
       
    if(increment) {
        PROGRAM_COUNTER+=2; 
    }
}