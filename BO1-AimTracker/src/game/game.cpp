#include "game.hpp"

#include <windows.h>

#include "process.hpp"

#include "../utils/memory.hpp"

#include <iomanip>

#include <sstream>

float aim_value = 0;

namespace game {
	float GetAimValue()
	{
		HANDLE handle = process::GetBlackOpsProcess();
		if (handle == NULL || handle == INVALID_HANDLE_VALUE)
		{
			return 0.0f;
		}

		return utils::memory::ReadFloat(handle, 0x2911E24);
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