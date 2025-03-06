#include <SFML/Graphics.hpp>

#include "game/game.hpp"

#include "resources/icon.h"

#include "resources/font.h"

#include <thread>

#include <atomic>

#include <sstream>

#include <iostream>

#include <windows.h>

std::atomic<bool> thread_running(true);

static void UpdateThread()
{
	while (thread_running)
	{
		game::UpdateAimValue();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// start update thread
	std::thread update_thread(UpdateThread);

	int window_width = 432;
	int window_height = 100;

	// load the icon
	sf::Image icon;
	if (!icon.loadFromMemory(resources::icon_png, resources::icon_png_len))
	{
		MessageBoxA(NULL, "Could not load asset \"icon_png\".", "BO1 Aim Tracker", MB_OK | MB_ICONERROR);
		return -1;
	}

	// load font
	sf::Font font;
	if (!font.loadFromMemory(resources::font_ttf, resources::font_ttf_len))
	{
		MessageBoxA(NULL, "Could not load asset \"font_ttf\".", "BO1 Aim Tracker", MB_OK | MB_ICONERROR);
		return -1;
	}

	// suggested direction text
	sf::Text suggested_direction_text("", font, 20);
	suggested_direction_text.setFillColor(sf::Color::White);

	// aim value text
	sf::Text aim_value_text("", font, 20);
	aim_value_text.setFillColor(sf::Color::White);

	// create the main_window
	sf::RenderWindow main_window(sf::VideoMode(window_width, window_height), L"BO1 Aim Tracker", sf::Style::Titlebar | sf::Style::Close);
	main_window.setFramerateLimit(10);

	// remove minimize button
	HWND hwnd = main_window.getSystemHandle();
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~WS_MINIMIZEBOX;
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	// set the icon
	main_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	while (main_window.isOpen())
	{
		// wait for the close button to be pressed
		sf::Event event;
		while (main_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				main_window.close();
			}
		}

		float y_offset = 20;

		// draw suggested direction
		suggested_direction_text.setString(game::GetSuggestedDirection());
		float suggested_direction_width = suggested_direction_text.getGlobalBounds().width;
		float suggested_direction_height = suggested_direction_text.getGlobalBounds().height;
		suggested_direction_text.setPosition((window_width - suggested_direction_width) / 2, ((window_height - suggested_direction_height) / 2) - y_offset);

		// draw aim value
		aim_value_text.setString(game::GetAimValueText());
		float aim_value_width = aim_value_text.getGlobalBounds().width;
		float aim_value_height = aim_value_text.getGlobalBounds().height;
		aim_value_text.setPosition((window_width - aim_value_width) / 2, ((window_height - aim_value_height) / 2) + (y_offset / 2));

		// draw everything to the main_window
		main_window.clear(sf::Color(5, 5, 5));
		main_window.draw(suggested_direction_text);
		main_window.draw(aim_value_text);
		main_window.display();
	}

	// clean up the update thread
	thread_running = false;
	update_thread.join();

	return 0;
}