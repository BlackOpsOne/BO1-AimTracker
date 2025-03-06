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
	int window_height = 135;

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
	sf::RenderWindow main_window(
		sf::VideoMode(window_width, window_height),
		L"BO1 Aim Tracker",
		sf::Style::Titlebar | sf::Style::Close
	);
	main_window.setFramerateLimit(10);

	// remove minimize button
	HWND hwnd = main_window.getSystemHandle();
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~WS_MINIMIZEBOX;
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	// set the icon
	main_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	// reset aim value button frame
	sf::RectangleShape reset_button_frame(sf::Vector2f(200, 35));
	reset_button_frame.setOutlineThickness(2);
	reset_button_frame.setOutlineColor(sf::Color::White);
	reset_button_frame.setFillColor(sf::Color::Transparent);
	reset_button_frame.setPosition((window_width - 200) / 2, 82);

	// reset aim value button text
	sf::Text reset_button_text("Reset Aim Value", font, 18);
	reset_button_text.setFillColor(sf::Color::White);
	float updateButtonTextWidth = reset_button_text.getGlobalBounds().width;
	float updateButtonTextHeight = reset_button_text.getGlobalBounds().height;
	reset_button_text.setPosition(
		reset_button_frame.getPosition().x + (reset_button_frame.getSize().x - updateButtonTextWidth) / 2,
		reset_button_frame.getPosition().y + (reset_button_frame.getSize().y - updateButtonTextHeight) / 2 - 5
	);

	while (main_window.isOpen())
	{
		// wait for the close button to be pressed
		sf::Event event;
		while (main_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				main_window.close();
			}

			// Check for mouse click on the Yes button
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mouse_position(event.mouseButton.x, event.mouseButton.y);
				if (reset_button_frame.getGlobalBounds().contains(mouse_position))
				{
					std::thread{ game::ResetAimValue }.detach();
				}
			}
		}

		// draw suggested direction
		suggested_direction_text.setString(game::GetSuggestedDirection());
		float suggested_direction_width = suggested_direction_text.getGlobalBounds().width;
		float suggested_direction_height = suggested_direction_text.getGlobalBounds().height;
		suggested_direction_text.setPosition((window_width - suggested_direction_width) / 2, 15);

		// draw aim value
		aim_value_text.setString(game::GetAimValueText());
		float aim_value_width = aim_value_text.getGlobalBounds().width;
		float aim_value_height = aim_value_text.getGlobalBounds().height;
		aim_value_text.setPosition((window_width - aim_value_width) / 2, 45);

		// change the text color and frame background colors
		// when the mouse is over the frame
		sf::Vector2i mouse_position = sf::Mouse::getPosition(main_window);
		if (reset_button_frame.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
			reset_button_frame.setFillColor(sf::Color(255, 255, 255));
			reset_button_text.setFillColor(sf::Color::Black);
		}
		else {
			reset_button_frame.setFillColor(sf::Color::Transparent);
			reset_button_text.setFillColor(sf::Color::White);
		}

		// draw everything to the main_window
		main_window.clear(sf::Color(5, 5, 5));
		main_window.draw(suggested_direction_text);
		main_window.draw(aim_value_text);
		main_window.draw(reset_button_frame);
		main_window.draw(reset_button_text);
		main_window.display();
	}

	// clean up the update thread
	thread_running = false;
	update_thread.join();

	return 0;
}