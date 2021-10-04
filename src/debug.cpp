#include "debug.h"
#include "app.h"

#define FONTS_PATH "resources/fonts/"

sf::Font* Debug::font = new sf::Font();
bool Debug::initialized = false;
sf::Text* Debug::text = new sf::Text();
std::vector<std::string> Debug::lines;
std::vector<std::string> Debug::persistentLines;

bool Debug::initialize()
{
	if (font->loadFromFile(std::string(FONTS_PATH) + "RobotoSlab-VariableFont_wght.ttf"))
	{
		text->setFont(*font);
		text->setCharacterSize(12);
		text->setFillColor(sf::Color::Green);
		text->setPosition(100.0f, 100.0f);
		initialized = true;
		return true;
	}
	else
	{
		return false;
	}
}

void Debug::draw(sf::RenderWindow* const window)
{

	// lazy initialization
	if (!initialized)
		if (!initialize())
			return;

	std::string newText;
	for (const auto& line : persistentLines)
	{
		newText.append(line + '\n');
	}
	for (const auto& line : lines)
	{
		newText.append(line + '\n');
	}
	lines.clear();
	text->setString(newText);
	text->setPosition(App::getWindowSize().x - text->getGlobalBounds().width - 10.0f, 10.0f);
	window->draw(*text);
}

void Debug::printPersistent(const std::string& line)
{
	persistentLines.push_back(line);
}

void Debug::clean()
{
	delete font;
	delete text;
}

void Debug::print(const std::string& line)
{
	lines.push_back(line);
}
