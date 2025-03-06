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

		bool WriteFloat(HANDLE handle, int address, float value)
		{
			if (address == -1 || handle == nullptr) {
				return false;
			}

			size_t bytes_written = 0;
			if (WriteProcessMemory(handle, (LPVOID)address, &value, sizeof(value), &bytes_written)
				&& bytes_written == sizeof(value)) {
				return true;
			}

			return false;
		}
	} // memory
} // utils