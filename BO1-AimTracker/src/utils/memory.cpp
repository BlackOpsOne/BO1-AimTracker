#include "memory.hpp"

namespace utils {
	namespace memory {
		float ReadFloat(HANDLE handle, int address)
		{
			if (address == -1 || handle == nullptr) {
				return 0.0f;
			}

			float buffer = 0.0f;
			size_t bytes_read = 0;
			if (ReadProcessMemory(handle, (LPCVOID)address, &buffer, sizeof(buffer), &bytes_read)
				&& bytes_read == sizeof(buffer)) {
				return std::round(buffer * 100.0f) / 100.0f;
			}

			return 0.0f;
		}
	} // memory
} // utils