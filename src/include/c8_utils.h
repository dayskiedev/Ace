#ifndef C8_UTILS
#define C8_UTILS

#include <iostream>
#include <bitset>
#include <iomanip>

// store a reference to emulator to peek memory / do other stuff

class c8_utils {
    public:
        // needs to get reference to memory to access it
        void CheckFont(const uint8_t CONST_MEMORY[], uint8_t startAddress, bool includeWhitespace);

        void CheckMemory(const uint8_t CONST_MEMORY[]);
        void CheckMemory(const uint8_t CONST_MEMORY[], uint8_t startAddress);
        void CheckMemory(const uint8_t CONST_MEMORY[], int startAddress, int endAddress);

};

#endif