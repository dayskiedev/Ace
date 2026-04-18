#include "c8_utils.h"

void c8_utils::CheckFont(const uint8_t  CONST_MEMORY[], uint8_t startAddress, bool includeWhitespace) {

    std::cout << "Checking font in memory starting at " << (int)startAddress << "\n";

    int _nlc = 0; // count for a new line (scuffed)
     for(uint8_t i = 0; i < 80; ++i) {
        std::string line = std::bitset<8>((int)CONST_MEMORY[i + startAddress]).to_string();
        if(!includeWhitespace) { line = line.substr(0,4); }
        std::cout << line << std::endl;
        _nlc++;
        if(_nlc == 5) {
            _nlc = 0;
            std::cout << std::endl;
        }
    }
};

void c8_utils::CheckMemory(const uint8_t CONST_MEMORY[], int startAddress, int endAddresss) {
    // uint8_t is 0 but int is 512?

    // move this to its own hex print function? really this could replace check font....
    int cols = 8;
    int spacing = 0;
    int count = 0;
    for(int i = startAddress; i < endAddresss; i++) {
        count++;
        spacing++;
        std::cout << std::setw(2) 
                  << std::hex 
                  << std::setfill('0') 
                  << (int)CONST_MEMORY[i];

        if(spacing == 2) {
            std::cout << " ";
            spacing = 0;
        }
        if(count == cols) {
            std::cout << std::endl;
            count = 0;
        }
    }
    std::cout << std::endl;
}