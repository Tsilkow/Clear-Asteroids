// Tsilkow

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"
#include "controller.hpp"
#include "crosshair.hpp"
#include "interface.hpp"
#include "scores.hpp"


using namespace std;

int main()
{
    srand(time(NULL));

    AsteroidSettings aSetts =
    {
	0.5f,  // m_invDensity
	32,   // TOSegments
	0.1f, // m_amplitude
	4,    // m_peakFrequency
	1,    // m_peakVariation
	0.3f  // m_peakAmplitude
    };

    ControllerSettings cSetts =
    {
	{2.f, 4.f},          // m_velocityRange
	{1000, 10000},       // m_massRange
	{-M_PI/60, M_PI/60}, // m_angVelocityRange
	{0.5f, 0.8f},        // m_colorLightRange
	800,                 // m_areaWidth
	800,                 // m_areaHeight
	50,                  // m_buffer
	2,                   // m_targetting
	60,                  // m_period
	0.5f                 // m_bounce
    };

    CrosshairSettings crSetts =
    {
	30,                                                     // m_cooldown
	10,                                                     // m_maneuverability
	{{0.f, 30.f}, {0.f, -30.f}, {30.f, 0.f}, {-30.f, 0.f}}, // m_shape
	sf::Color(255,   0,   0),                               // m_normColor
	sf::Color(255, 255, 255)                                // m_coolColor
    };

    StationSettings sSetts =
    {
	25,         // m_radius
	M_PI/600.f  // m_angVelocity
    };

    std::string scoreFilename = "scores.json";
    Scores scores(10);
    if(!scores.load(scoreFilename))
    {
	cout << "Error! Could not read score file. Generating an empty one" << endl;
    }

    for(int i = 0; i < 20; ++i)
    {
	int score = randomI(0, 100);
	cout << score << " ---> " << scores.isScoreSignificant(score) << " "
	     << scores.addScore(to_string(i), score) << endl;
    }

    scores.save(scoreFilename);

    std::string fontFilename = "Sicretmono.ttf";
    sf::Font font;
    if(!font.loadFromFile(fontFilename))
    {
	cout << "!ERROR! Font could not be loaded! Aborting ..." << endl;
	return 1;
    }
    shared_ptr<sf::Font> shr_font = make_shared<sf::Font>(font);

    shared_ptr<AsteroidSettings> shr_aSetts = make_shared<AsteroidSettings>(aSetts);
    shared_ptr<ControllerSettings> shr_cSetts = make_shared<ControllerSettings>(cSetts);
    shared_ptr<CrosshairSettings> shr_crSetts = make_shared<CrosshairSettings>(crSetts);
    shared_ptr<StationSettings> shr_sSetts = make_shared<StationSettings>(sSetts);

    shr_sSetts->m_texture.loadFromFile("station.png");

    sf::RenderWindow window(sf::VideoMode(800, 800), "Clear Asteroids");
    window.setFramerateLimit(60);

    Controller controller(shr_aSetts, shr_cSetts, shr_sSetts);
    Crosshair crosshair(shr_crSetts);
    Interface menuInterface(font);
    Interface playInterface(font);
    Interface scoresInterface(font);

    playInterface.addString("0.00", sf::Vector2f(-390, -400), -1, 50);
    playInterface.addString("0", sf::Vector2f(325, -400), 0, 50); 
    playInterface.addString("0", sf::Vector2f(325, -350), 0, 50); 
    playInterface.addString(" ", sf::Vector2f(325, -300), 0, 50);

    sf::View actionView(sf::Vector2f(0.f, 0.f), sf::Vector2f(800, 800));
    window.setView(actionView);

    enum GameState{Menu, Play, Scores};
    GameState currState = Play;
    bool hasFocus = true;
    int ticksPassed = 0;
    int shotCount = 0;
    int killCount = 0;

    while(window.isOpen())
    {
	sf::Event event;
	
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
			    //std::cout << "LOST FOCUS" << std::endl;
			    break;
			case sf::Event::GainedFocus:
			    hasFocus = true;
			    //std::cout << "GAINED FOCUS" << std::endl;
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
		    if(!controller.tick(ticksPassed)) cout << "Station's been hit" << endl;
		    crosshair.tick(ticksPassed, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		    playInterface.setContent(0, trailingZeroes((float)ticksPassed/60.f, 2));
		    playInterface.setContent(1, std::to_string(killCount));
		    playInterface.setContent(2, std::to_string(shotCount));
		    if(shotCount >= 10)
		    {
			playInterface.setContent(3, trailingZeroes(100.f *(float)killCount/
								   (float)shotCount, 1) + "%");
		    }

		    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		       crosshair.shoot(ticksPassed))
		    {
			++shotCount;
			killCount += controller.destroyAt(crosshair.getPosition());
		    }
		    ++ticksPassed;
		}
		else
		{
		    ;
		}
		controller.draw(window);
		crosshair.draw(window);
		playInterface.draw(window);
		break;
	    case GameState::Scores:
		break;
	}
	window.display();
    }
    
    return 0;
}
