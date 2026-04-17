#ifndef C8_UTILS
#define C8_UTILS

#include <iostream>
#include <bitset>

// store a reference to emulator to peek memory

class c8_utils {
    public:
        // needs to get reference to memory to access it
        void CheckFont(uint8_t MEMORY[], uint8_t startAddress, bool includeWhitespace);
};

#endif