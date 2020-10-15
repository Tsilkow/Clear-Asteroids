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
    Interface pauseInterface(shr_font, sf::Vector2f(800, 800), sf::Color (0, 0, 0, 255));

    menuInterface.addTextBox("intro", "... and then, there was only one objective before them:",
			  sf::Vector2f(0, -310), 20, 0, sf::Color::White, 2);
    menuInterface.addTextBox("title", "CLEAR ASTEROIDS", sf::Vector2f(0, -300), 100, 0, sf::Color::White, 1);
    menuInterface.addTextBox("tPlay", "Play"  , sf::Vector2f(0, -50), 60, 0);
    menuInterface.addButton("bPlay", "tPlay", sf::FloatRect(0, 0, 0, 0));
    menuInterface.addTextBox("tScores", "Scores", sf::Vector2f(0,  50), 60, 0);
    menuInterface.addButton("bScores", "tScores", sf::FloatRect(0, 0, 0, 0));
    menuInterface.addTextBox("tExit", "Exit", sf::Vector2f(0,  150), 60, 0);
    menuInterface.addButton("bExit", "tExit", sf::FloatRect(0, 0, 0, 0));

    playInterface.addTextBox("timer", "0.00", sf::Vector2f(-390, -400), 50, -1);
    playInterface.addTextBox("kills", "0", sf::Vector2f(325, -400), 50, 0); 
    playInterface.addTextBox("shots", "0", sf::Vector2f(325, -350), 50, 0); 
    playInterface.addTextBox("efficiency", " ", sf::Vector2f(325, -300), 50, 0);
    
    scoresInterface.addTextBox("title", "Highscores", sf::Vector2f(0, -300), 60, 0);
    for(int i = 0; i < 10; ++i)
    {
	string numeral = std::to_string(i+1) + ". ";
	if(i != 9) numeral = " " + numeral;
	scoresInterface.addTextBox(std::to_string(i+1)          , numeral,
				   sf::Vector2f(-250, -225 + 50*i), 50, -1);
	scoresInterface.addTextBox(std::to_string(i+1) + "name" , "", 
				   sf::Vector2f(-150, -225 + 50*i), 50, -1);
	scoresInterface.addTextBox(std::to_string(i+1) + "score",  "", 
				   sf::Vector2f(200, -225 + 50*i), 50, -1);
    }
    scoresInterface.addTextBox("tBack", "Back", sf::Vector2f(0, 300), 60, 0);
    scoresInterface.addButton("bBack", "tBack", sf::FloatRect(0, 0, 0, 0));

    pauseInterface.addTextBox("Paused", "Paused", sf::Vector2f(0, -100.f), 100, 0);

    sf::View actionView(sf::Vector2f(0.f, 0.f), sf::Vector2f(800, 800));
    window.setView(actionView);

    enum GameState{Menu, Play, Scores};
    GameState currState = GameState::Menu;
    bool hasFocus = true;
    bool start = true;
    bool newScore = false;
    bool firstTimeInScores = true;
    int ticksPassed = 0;
    int playStart = -1;
    int shotCount = 0;
    int killCount = 0;
    int score = 0;
    int lastScore = 0;

    while(window.isOpen())
    {
	sf::Event event;
	std::pair<std::string, std::string> input;
	
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

	    if(hasFocus)
	    {
		switch(currState)
		{
		    case GameState::Menu:
			input = menuInterface.tick(ticksPassed,
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
			    else if(input.first == "bExit")
			    {
				window.close();
			    }
			}
			break;
		    
		    case GameState::Play:
			input = playInterface.tick(ticksPassed,
						   window.mapPixelToCoords(sf::Mouse::getPosition(window)),
						   event);
			break;
		    
		    case GameState::Scores:
			input = scoresInterface.tick(ticksPassed,
						     window.mapPixelToCoords(sf::Mouse::getPosition(window)),
						     event);

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
			    if(input.first == "bBack")
			    {
				currState = GameState::Menu;
			    }
			}

			if(input.second != "")
			{
			    scores.addScore(input.second, score);
			    vector< vector<string> > lines = scores.potenList(-1);
			    for(int i = 0; i < 10; ++i)
			    {
				scoresInterface.changeTextBox(std::to_string(i+1) + "name" , lines[0][i]);
				scoresInterface.changeTextBox(std::to_string(i+1) + "score", lines[1][i]); 
			    }
			    scores.save(scoreFilename);
			}
			break;
		    
		}
	    }
	}
	
	switch(currState)
	{
	    case GameState::Menu:
		if(hasFocus)
		{
		    controller.tick(false, ticksPassed);
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
			killCount = 0;
			shotCount = 0;
			lastScore = score;
			controller.killStation();
			if(scores.potenPlace(score) >= 0)
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

		    if(ticksPassed - playStart > 10 &&
		       sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
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
		if(!hasFocus) pauseInterface.draw(window);
		break;
		
	    case GameState::Scores:
		int place = scores.potenPlace(score);
		if(newScore || firstTimeInScores)
		{
		    vector< vector<string> > lines = scores.potenList(-1 + (score+1) * newScore);
		
		    for(int i = 0; i < lines[1].size(); ++i)
		    {
			scoresInterface.changeTextBox(std::to_string(i+1) + "name" , lines[0][i]);
			scoresInterface.changeTextBox(std::to_string(i+1) + "score", lines[1][i]); 
		    }
		    
		    if(newScore) scoresInterface.addInputBox("newScore", std::to_string(place+1) + "name");
		    newScore = false;
		    firstTimeInScores = false;
		}
		
		if(hasFocus)
		{
		    controller.tick(false, ticksPassed);
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
