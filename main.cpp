#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "asteroids.hpp"


using namespace std;

int main()
{
    srand(time(NULL));

    AsteroidSettings aSetts =
    {
	1.f,
	{10.f, 20.f},
	{100, 1000},
	{-M_PI/3, M_PI/3},
	16,
	{0.5f, 0.8f},
	0.1f,
	4,
	1,
	0.3f
    };

    shared_ptr<AsteroidSettings> shr_aSetts = make_shared<AsteroidSettings>(aSetts);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Clear Asteroids");
    window.setFramerateLimit(60);
    
    cout << "I saw red venting" << endl;

    vector<Asteroid> asteroids;

    
    
    asteroids.emplace_back(shr_aSetts, 2000, sf::Color(255, 128, 0), sf::Vector2f(0.f, 0.f), sf::Vector2f(1.f, 1.f), (float)M_PI/15.f);

    enum GameState{Menu, Play, Scores};
    GameState currState = Play;

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
		    for(int i = 0; i < asteroids.size(); ++i)
		    {
			asteroids[i].tick();
		    }
		}
		else
		{
		    ;
		}
		
		for(int i = 0; i < asteroids.size(); ++i)
		{
		    asteroids[i].draw(window);
		}
		
		break;
	    case GameState::Scores:
		break;
	}
	window.display();
    }
    
    return 0;
}
