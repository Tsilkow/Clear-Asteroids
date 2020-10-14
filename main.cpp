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
	{4.f, 8.f},          // m_velocityRange
	{1000, 10000},       // m_massRange
	{-M_PI/60, M_PI/60}, // m_angVelocityRange
	{0.5f, 0.8f},        // m_colorLightRange
	800,                 // m_areaWidth
	800,                 // m_areaHeight
	50,                  // m_buffer
	2,                   // m_targetting
	60,                  // m_startAPM - starting value ofasteroids per minute
	60,                  // m_APMincrease - APM increase per minute
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
    Interface menuInterface(shr_font, sf::Vector2f(800, 800), sf::Color(0, 0, 0, 192));
    Interface playInterface(shr_font, sf::Vector2f(800, 800), sf::Color(0, 0, 0, 0));
    Interface scoresInterface(shr_font, sf::Vector2f(800, 800), sf::Color(0, 0, 0, 192));

    menuInterface.addTextBox("intro", "... and then, there was only one objective before them:",
			  sf::Vector2f(0, -310), 20, 0, sf::Color::White, 2);
    menuInterface.addTextBox("title", "CLEAR ASTEROIDS", sf::Vector2f(0, -300), 100, 0, sf::Color::White, 1);
    menuInterface.addTextBox("tPlay", "Play"  , sf::Vector2f(0, -50), 60, 0);
    menuInterface.addButton("bPlay", "tPlay", sf::FloatRect(0, 0, 0, 0));
    menuInterface.addTextBox("tScores", "Scores", sf::Vector2f(0,  50), 60, 0);
    menuInterface.addButton("bScores", "tScores", sf::FloatRect(0, 0, 0, 0));

    playInterface.addTextBox("timer", "0.00", sf::Vector2f(-390, -400), 50, -1);
    playInterface.addTextBox("kills", "0", sf::Vector2f(325, -400), 50, 0); 
    playInterface.addTextBox("shots", "0", sf::Vector2f(325, -350), 50, 0); 
    playInterface.addTextBox("efficiency", " ", sf::Vector2f(325, -300), 50, 0);
    
    scoresInterface.addTextBox("title", "Highscores", sf::Vector2f(0, -300), 60, 0);
    scoresInterface.addTextBox("1. place",  " 1.                         ", sf::Vector2f(0, -225), 50, 0);
    scoresInterface.addTextBox("2. place",  " 2.                         ", sf::Vector2f(0, -175), 50, 0);
    scoresInterface.addTextBox("3. place",  " 3.                         ", sf::Vector2f(0, -125), 50, 0);
    scoresInterface.addTextBox("4. place",  " 4.                         ", sf::Vector2f(0,  -75), 50, 0);
    scoresInterface.addTextBox("5. place",  " 5.                         ", sf::Vector2f(0,  -25), 50, 0);
    scoresInterface.addTextBox("6. place",  " 6.                         ", sf::Vector2f(0,   25), 50, 0);
    scoresInterface.addTextBox("7. place",  " 7.                         ", sf::Vector2f(0,   75), 50, 0);
    scoresInterface.addTextBox("8. place",  " 8.                         ", sf::Vector2f(0,  125), 50, 0);
    scoresInterface.addTextBox("9. place",  " 9.                         " , sf::Vector2f(0,  175), 50, 0);
    scoresInterface.addTextBox("10. place", "10.                         ", sf::Vector2f(0,  225), 50, 0);
    scoresInterface.addTextBox("tBack", "Back", sf::Vector2f(0, 300), 60, 0);
    scoresInterface.addButton("bBack", "tBack", sf::FloatRect(0, 0, 0, 0));

    sf::View actionView(sf::Vector2f(0.f, 0.f), sf::Vector2f(800, 800));
    window.setView(actionView);

    enum GameState{Menu, Play, Scores};
    GameState currState = GameState::Menu;
    bool hasFocus = true;
    bool start = true;
    bool newScore = false;
    int ticksPassed = 0;
    int playStart = -1;
    int shotCount = 0;
    int killCount = 0;
    int score = 0;
    int lastScore = 0;

    while(window.isOpen())
    {
	sf::Event event;
	
	window.clear();
	
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
	
	switch(currState)
	{
	    case GameState::Menu:
		if(hasFocus)
		{
		    controller.tick(false, ticksPassed);

		    auto input = menuInterface.tick(ticksPassed,
						    window.mapPixelToCoords(sf::Mouse::getPosition(window)),
						    event);

		    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		    {
			if(input.first == "bPlay")
			{
			    currState = GameState::Play;
			    playStart = -1;
			}
			else if(input.first == "bScores")
			{
			    currState = GameState::Scores;
			}
		    }
		}
		
		controller.draw(window);
		menuInterface.draw(window);
		break;
		
	    case GameState::Play:
		if(playStart == -1)
		{
		    playStart = ticksPassed;
		    controller.start();
		}

		if(hasFocus)
		{
		    score = ticksPassed - playStart;
		    if(!controller.tick(true, ticksPassed))
		    {
			score += 0.5f*score * (float)killCount/(float)shotCount;
			lastScore = score;
			controller.killStation();
			if(scores.isScoreSignificant(score) >= 0)
			{
			    currState = GameState::Scores;
			    newScore = true;
			}
			else currState = GameState::Menu;
			break;
		    }
		    crosshair.tick(ticksPassed, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		    playInterface.changeTextBox("timer",
						trailingZeroes((float)(ticksPassed - playStart)/60.f, 2));
		    playInterface.changeTextBox("kills",
						std::to_string(killCount));
		    playInterface.changeTextBox("shots",
						std::to_string(shotCount));
		    if(shotCount >= 10)
		    {
			playInterface.changeTextBox("efficiency",
						    trailingZeroes(100.f *(float)killCount/
								   (float)shotCount, 1) + "%");
		    }

		    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		       crosshair.shoot(ticksPassed))
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
		playInterface.draw(window);
		break;
		
	    case GameState::Scores:
		if(newScore)
		{
		    newScore = false;
		}
		
		if(hasFocus)
		{
		    controller.tick(false, ticksPassed);

		    auto input = scoresInterface.tick(ticksPassed,
						      window.mapPixelToCoords(sf::Mouse::getPosition(window)),
						      event);

		    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		    {
			if(input.first == "bBack")
			{
			    currState = GameState::Menu;
			}
		    }
		}
		
		controller.draw(window);
		scoresInterface.draw(window);
		break;
	}

	if(hasFocus)
	{
	    ++ticksPassed;
	}
	
	window.display();
    }
    
    return 0;
}
