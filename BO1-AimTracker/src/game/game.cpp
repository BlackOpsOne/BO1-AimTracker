#include "game.hpp"

#include <windows.h>

#include "process.hpp"

#include "../utils/memory.hpp"

#include <iomanip>

#include <sstream>

float aim_value = 0;

int aim_value_address = 0x2911E24;

namespace game {
	float GetAimValue()
	{
		HANDLE handle = process::GetBlackOpsProcess();
		if (handle == NULL || handle == INVALID_HANDLE_VALUE)
		{
			return 0.0f;
		}

		return utils::memory::ReadFloat(handle, aim_value_address);
	}

	void UpdateAimValue()
	{
		HANDLE handle = process::GetBlackOpsProcess();
		if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
			aim_value = 0;
			return;
		}

		aim_value = GetAimValue();
	}

	void ResetAimValue()
	{
		HANDLE handle = process::GetBlackOpsProcess();
		if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
			MessageBoxA(NULL, "The game must be open to reset the aim value.", "BO1 Aim Tracker", MB_OK | MB_ICONINFORMATION);
			return;
		}

		if (!utils::memory::WriteFloat(handle, aim_value_address, 0.0f))
		{
			MessageBoxA(NULL, "Failed to reset the aim value.", "BO1 Aim Tracker", MB_OK | MB_ICONINFORMATION);
			return;
		}

		MessageBoxA(NULL, "Successfully reset the aim value.", "BO1 Aim Tracker", MB_OK | MB_ICONINFORMATION);
	}

	std::string GetSuggestedDirection()
	{
		std::string direction_suggestion = "";

		if (!game::process::IsGameOpen())
		{
			return "The game is not open";
		}

		int turning_suggestion_threshold = 200000;

		if (aim_value < -turning_suggestion_threshold)
		{
			return "It is suggested to turn left";
		}
		else if (aim_value > turning_suggestion_threshold)
		{
			return "It is suggested to turn right";
		}
		else
		{
			return "No turning required";
		}
	}

	std::string GetAimValueText()
	{
		// in furret's aim tracker program, the value does not appear to be formatted
		// so we'll format it with only two decimal points for better readability
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(2) << aim_value;
		return "Aim Value: " + stream.str();
	}
}