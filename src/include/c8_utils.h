#ifndef C8_UTILS
#define C8_UTILS

#include <iostream>
#include <bitset>

// store a reference to emulator to peek memory / do other stuff

class c8_utils {
    public:
        // needs to get reference to memory to access it
        void CheckFont(const uint8_t CONST_MEMORY[], uint8_t startAddress, bool includeWhitespace);
};

#endif