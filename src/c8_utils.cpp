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