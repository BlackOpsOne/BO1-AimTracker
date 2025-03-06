#include <windows.h>
#include <string>

namespace game {
	namespace process {
		HANDLE GetBlackOpsProcess();
		bool IsGameOpen();
		DWORD GetProcessIdByName(const std::wstring& procName);
	} // process
} // game