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

    // set random value seed
    std::srand(_RAND_SEED);

    return true;
}


// All this is used for is to keep track of delay and sound timers
// will run independent of Cycle, so if we wait for input, tick will still run
// shoud run at 60hz aka 60 times a second (look into deltatime?)
void c8_emulator::Tick() {

}


void c8_emulator::Cycle() {    
    // timing should be configurable 700 instructions a second should be alright.

    // [fetch]
    uint16_t opcode = MEMORY[PROGRAM_COUNTER];

    if(PROGRAM_COUNTER > START_ADR + romSize) {
        std::cout << "Outside ROM Memory, terminate program.\n";
        return;
    }

    opcode <<= 8;
    opcode |= MEMORY[PROGRAM_COUNTER+1];

    // immediatly increment PC to be ready for the next instruction
    PROGRAM_COUNTER+=2; 

    // [decode]
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

    // Used for draw instruction (probably should not be doing this for EVERY instruction but switch state complains...)
    uint8_t X;
    uint8_t Y;

    // 0x8XY8 made as 16bit so it can store value if it goes over the limit
    uint16_t addValue;

    //std::cout << n1 << "|" << n2 << "|" << n3 << "|" << n4 << std::endl;
    std::cout << "Raw opcode: " << std::hex << "0x" << std::uppercase << std::setw(4) << std::setfill('0') << opcode << std::dec << " | ";   
    
    // [execute]
    switch (n1) {
    case 0x0:
        switch (opcode) {
        case 0x00E0:
            std::cout << "Clear screen\n";
            std::fill(VIDEO, VIDEO+((64*32)), PIXEL_OFF);
            break;
        case 0x00EE:
            // pop last address from stack setting PC to it.
            std::cout << "Pop from top of Address Stack and set PC to its value\n";
            PROGRAM_COUNTER = ADDRESS_STACK.top();
            ADDRESS_STACK.pop();
            break;
        }
        break;

    case 0x1:
        // we should not be incrementing the pc counter after this jump
        std::cout << "Jump to: " << std::hex << NNN <<  std::dec << std::endl;
        PROGRAM_COUNTER = NNN;
        break;
    case 0x2:
        // call subroutine at memory NNN first push current PC tho
        std::cout << "Push current address to ADDRESS_STACK and set PC to  " << std::hex << NNN <<  std::dec << std::endl;
        ADDRESS_STACK.push(PROGRAM_COUNTER);
        PROGRAM_COUNTER = NNN;
        break;
    case 0x3:
        std::cout << "Skip 1 instruction if the value in V" << std::hex << n2 << " is equal to " << NN << std::dec << std::endl;
        if(REGISTERS[n2] == NN) {
            PROGRAM_COUNTER+=2; // remember an instruction is 2 bytes, so we have to skip twice (could just set increment to true?)
        }
        break;
    case 0x4:
        // skip if vx != nn
        if(REGISTERS[n2] != NN) {
            PROGRAM_COUNTER+=2;
        }
        break;
    case 0x5:
        // skip if vx == vy
        if(REGISTERS[n2] == REGISTERS[n3]) {
            PROGRAM_COUNTER+=2;
        }
        break;
    case 0x6:
        std::cout << std::hex << "Set register V" << n2 << " to " << std::hex << NN << std::dec << "\n";
        REGISTERS[n2] = NN;
        //std::cout << REGISTERS[n2] << " < val of reg" << std::endl;
        break;
    case 0x7:
        std::cout << "Add value " << std::hex << NN << std::dec << " to register V" << n2 << "\n";
        REGISTERS[n2] += NN;
        break;

    // 8XYN
    case 0x8:
        switch (n4)
        {
        case 0x0:
            // set vx to vy
            std::cout << "Set V" << std::hex << n2 << " To V" << n3 << std::endl;
            REGISTERS[n2] = REGISTERS[n3];
            break;
        case 0x1:
            // set vx to vx | vy
            std::cout << "Set V" << std::hex << n2 << " To an OR bitwise with V" << n3 << std::endl;
            REGISTERS[n2] |= REGISTERS[n3];
            break;
        case 0x2:
            // Binary AND
            std::cout << "Set V" << std::hex << n2 << " To an AND bitwise with V" << n3 << std::endl;
            REGISTERS[n2] &= REGISTERS[n3];
            break;
        case 0x3:
            // Logical XOR
            std::cout << "Set V" << std::hex << n2 << " To an XOR bitwise with V" << n3 << std::endl;
            REGISTERS[n2] ^= REGISTERS[n3];
            break;
        case 0x4:
            std::cout << "Set V" << std::hex << n2 << " To itself plus V" << n3 << std::endl;
            addValue = REGISTERS[n2] + REGISTERS[n3];
            REGISTERS[n2] = addValue;
            // unlike with 0x7, if this value exceeds 255, we indicate the overflow with flipping vf to 1
            REGISTERS[15] = 0;         
            if(addValue > 255) {
                REGISTERS[15] = 1;
            } 
            break;
        case 0x5:
            std::cout << "Set V" << std::hex << n2 << " To itself minus V" << n3 << std::endl;
            // check for underflow, set vf flag if this occurs
            REGISTERS[15] = 1;
            if(n2 < n3) { REGISTERS[15] = 0; }
            REGISTERS[n2] -= REGISTERS[n3];
            break;
        case 0x6:
            // put the value of vy int vx should be configurable
            std::cout << "Set V" << std::hex << n2 << " to V" << n3 << "(should be toggled, and shift by 1 to the right)" << std::endl; 
            REGISTERS[n2] = REGISTERS[n3];
            // set vf to the first bit (which will be shifted out)
            REGISTERS[15] = REGISTERS[n2] & 0x1;
            REGISTERS[n2] >>= 1;
            break;
        case 0x7:
            std::cout << "Set V" << std::hex << n2 << " To V" << n3 << " minus itself" << std::endl;
            REGISTERS[15] = 1;
            if(n3 < n2) { REGISTERS[15] = 0; }
            REGISTERS[n2] = REGISTERS[n3] = REGISTERS[n2];
            break;
        case 0xE:
            std::cout << "Set V" << std::hex << n2 << " to V" << n3 << "(should be toggled, and shift by 1 to the left)" << std::endl; 
            // this should be optional 
            REGISTERS[n2] = REGISTERS[n3];
            // get left most bit 
            REGISTERS[15] = REGISTERS[n2] & 0x80; 
            REGISTERS[n2] <<=1;
            break;
        default:
            std::cout << "Unknown 8XYN nibble.\n";
            break;
        }    
    case 0x9:
        // skip if vx is not equal to vy
        if(REGISTERS[n2] != REGISTERS[n3]) {
            PROGRAM_COUNTER+=2;
        }
        break;
    
    case 0xA:
        std::cout  << "Set index register I to " << std::hex << NNN << std::dec << "\n";
        INDEX_REGISTER = NNN;
        break;
    case 0xC:
        // this also may indicate an end of file.
        std::cout << "Generate a random number, AND it with " << NN << " and place in register VX\n";
        REGISTERS[n2] = NN &  (std::rand() % 128);
        break;
    case 0xD:
        std::cout << "Display/Draw\n";
        X = REGISTERS[n2] % 64;
        Y = REGISTERS[n3] % 32;
        REGISTERS[15] = 0;

        for(uint16_t i = 0; i < N; i++) {
            uint8_t spriteData = MEMORY[INDEX_REGISTER + i];
            for(int b = 0; b < 8; ++b) {
                // scroll guard if we reach over 64, we don't want to draw anything (may cause issues...)
                if(X >= 64) { continue; } 

                // get most significant bit from spirteData, move it to first index so its either 1 or 0
                uint8_t bit = (spriteData & 0x80) >> 7;

                // move next bit to most significant position
                spriteData <<= 1; // shift left by 1

                // convert 2d coords to 1D array, (Y just tells us the row number to search for where x points to the col)
                int position = X + (Y * 64); 
                
                // probably a better way to do this?
                // XOR FF = false, TT = false, TF = True, FT = true
                if(bit == 1) {
                    if(VIDEO[position] == PIXEL_ON) {
                        VIDEO[position] = PIXEL_OFF;
                        REGISTERS[15] = 1; // set vf to 1
                    } else {
                        VIDEO[position] = PIXEL_ON;
                    }
                } 
                X++;
            }
            Y++;
            X = REGISTERS[n2] % 64;

        }
        break;
    default:
        std::cout << "Unknown Nibble... \n";
        break;
    }
    
    // should be before we do instruction stuff for some reason?
}