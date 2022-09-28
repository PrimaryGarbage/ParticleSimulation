#include "app.h"
#include "debug.h"

typedef unsigned int uint;

sf::RenderWindow* App::window = nullptr;
uint App::windowWidth = 0u;
uint App::windowHeight = 0u;
double App::elapsedTime = 0.0;
float App::deltaTime = 0.0;
PhysicsSolver App::phSolver;

bool App::initialize(unsigned int width, unsigned int height, const char* name) noexcept
{
	windowWidth = width;
	windowHeight = height;
	window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(width, height)), name, sf::Style::Titlebar | sf::Style::Close);
	window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(60u);
	window->setKeyRepeatEnabled(false);

    if (window) return true;
    else return false;
}

void App::run()
{
    sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));

	sf::Shader testShader;
	testShader.loadFromFile("./src/shaders/test_shader.frag", sf::Shader::Fragment);
	testShader.setUniform("windowSize", sf::Vector2f(getWindowSize()));


	sf::Clock clock;

    while (window->isOpen())
    {
		deltaTime = clock.restart().asSeconds();
		elapsedTime += deltaTime;

		//testShader.setParameter("elapsedTime", elapsedTime);

        sf::Event event;
        while (window->pollEvent(event))
        {
            processEvent(event);
        }


        /////// GAME LOGIC////////
        getInput();

        phSolver.update(deltaTime);
        //////////////////////////


        window->clear();

        /////// DRAWING ///////
        window->draw(background, &testShader);
        phSolver.renderParticles(window);
        Debug::draw(window);
        ///////////////////////

        window->display();
    }
}

void App::processEvent(const sf::Event& event)
{
    switch(event.type)
    {

        case sf::Event::Closed:
            {
                window->close();
                break;
            }

        case sf::Event::KeyPressed:
            {
                switch(event.key.code)
                {
                case sf::Keyboard::T:
                    phSolver.particleTrails = !phSolver.particleTrails;
                    break;

                default:
                    break;
                }
            }

        default:
            break;

    }
}


void App::getInput()
{
    if(window->hasFocus())
    {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
            //testShader.setParameter("mousePos", mousePosition);
            Debug::print(std::to_string(mousePosition.x) + "|" + std::to_string(mousePosition.y));
            phSolver.addParticle(ParticleType::Electron, mousePosition);
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
            //testShader.setParameter("mousePos", mousePosition);
            Debug::print(std::to_string(mousePosition.x) + "|" + std::to_string(mousePosition.y));
            phSolver.addParticle(ParticleType::Proton, mousePosition);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            phSolver.clearParticles();
        }
    }
}

void App::close() noexcept
{
	delete window;
}

sf::Vector2i App::getWindowSize()
{
    return sf::Vector2i(windowWidth, windowHeight);
}
