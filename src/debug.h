#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include <vector>

#define PLACE "File: " + __FILE__ + ". Line: " + std::to_string(__LINE__) + "."

class Debug
{
private:
	static sf::Font* font;
	static bool initialized;
	static std::vector<std::string> lines;
	static std::vector<std::string> persistentLines;
	static sf::Text* text;

	static bool initialize();
public:
	static void draw(sf::RenderWindow* const window);
	static void print(const std::string& line);
	static void printPersistent(const std::string& line);
	static void clean();
};

