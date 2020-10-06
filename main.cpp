// Tsilkow

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "controller.hpp"
#include "crosshair.hpp"


using namespace std;

int main()
{
    srand(time(NULL));

    AsteroidSettings aSetts =
    {
	1.f,
	32,
	0.1f,
	4,
	1,
	0.3f
    };

    ControllerSettings cSetts =
    {
	{2.f, 4.f},
	{1000, 10000},
	{-M_PI/60, M_PI/60},
	{0.7f, 0.8f},
	800,
	800,
	50,
	60,
	0.5f
    };

    CrosshairSettings crSetts =
    {
	30,
	10,
	{{0.f, 30.f}, {0.f, -30.f}, {30.f, 0.f}, {-30.f, 0.f}},
	sf::Color(255,   0,   0),
	sf::Color(255, 255, 255)
    };

    shared_ptr<AsteroidSettings> shr_aSetts = make_shared<AsteroidSettings>(aSetts);
    shared_ptr<ControllerSettings> shr_cSetts = make_shared<ControllerSettings>(cSetts);
    shared_ptr<CrosshairSettings> shr_crSetts = make_shared<CrosshairSettings>(crSetts);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Clear Asteroids");
    window.setFramerateLimit(60);

    Controller controller(shr_aSetts, shr_cSetts);
    Crosshair crosshair(shr_crSetts);

    sf::View actionView(sf::Vector2f(0.f, 0.f), sf::Vector2f(800, 800));
    window.setView(actionView);

    enum GameState{Menu, Play, Scores};
    GameState currState = Play;
    int ticksPassed = 0;
    int shotCount = 0;
    int killCount = 0;

    while(window.isOpen())
    {
	sf::Event event;
	bool hasFocus = true;
	
	window.clear();
	switch(currState)
	{
	    case GameState::Menu:
		break;
	    case GameState::Play:
		while (window.pollEvent(event))
		{
		    switch(event.type)
		    {
			case sf::Event::Closed:
			    window.close();
			    break;
			case sf::Event::LostFocus:
			    hasFocus = false;
			    break;
			case sf::Event::GainedFocus:
			    hasFocus = true;
			    break;
			case sf::Event::KeyPressed:
			    if(hasFocus)
			    {
				switch(event.key.code)
				{
				    case sf::Keyboard::Escape:
					window.close();
					break;
				}
			    }
			    break;
		    }
		}

		if(hasFocus)
		{
		    controller.tick(ticksPassed);
		    crosshair.tick(ticksPassed, window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && crosshair.shoot(ticksPassed))
		    {
			++shotCount;
			killCount += controller.destroyAt(crosshair.getPosition());
		    }
		}
		else
		{
		    ;
		}
		controller.draw(window);
		crosshair.draw(window);
		++ticksPassed;
		break;
	    case GameState::Scores:
		break;
	}
	window.display();
    }
    
    return 0;
}
