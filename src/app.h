#pragma once
#include <SFML/Graphics.hpp>

class App
{
private:
	static sf::RenderWindow* window;
	static unsigned int windowWidth;
	static unsigned int windowHeight;
	static double elapsedTime;
	static float deltaTime;
public:
	App() = delete;
	App(const App& other) = delete;
	App(App&& other) = delete;
	void operator=(const App& other) = delete;
	static bool initialize(unsigned int width, unsigned int height, const char* name) noexcept;
	static void run();
	static void close() noexcept;

	static sf::Vector2i getWindowSize();
};
