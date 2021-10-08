#include <SFML/Graphics.hpp>
#include "app.h"

using uint = unsigned int;

int main()
{
	const uint windowWidth = 800u;
	const uint windowHeight = 800u;
	const char windowName[] = "Particles";

	if(App::initialize(windowWidth, windowHeight, windowName))
	{
		App::run();
		App::close();
		return 0;
	}
	else
	{
		return 1;
	}
}
