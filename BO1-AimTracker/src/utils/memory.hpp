#include <windows.h>
#include <iostream>

namespace utils {
    namespace memory {
        float ReadFloat(HANDLE handle, int address);
        bool WriteFloat(HANDLE handle, int address, float value);
    } // memory
} // utils