#pragma once

#include <iostream>
#include <vector>
#include "asteroid.hpp"
#include "station.hpp"


struct ControllerSettings
{
    // asteroid creation settings
    std::vector<float> m_velocityRange;
    std::vector<int> m_massRange;
    std::vector<float> m_angVelocityRange; //angular velocity
    std::vector<float> m_colorLightRange;
    int m_areaWidth;
    int m_areaHeight;
    int m_buffer;
    int m_targetting;

    // asteroid simulation settings
    int m_period; // time between creating
    float m_bounce;
};

class Controller
{
    private:
    std::vector<Asteroid> m_asteroids;
    Station m_station;
    std::shared_ptr<AsteroidSettings> m_aSetts;
    std::shared_ptr<ControllerSettings> m_cSetts;
    int m_lastAstCreated; // tick when last asteroid was created
    sf::FloatRect m_bounds;

    void createAsteroid();
    
    public:
    Controller(std::shared_ptr<AsteroidSettings> aSetts, std::shared_ptr<ControllerSettings> cSetts,
	       std::shared_ptr<StationSettings> sSetts);

    bool tick(int ticksPassed);

    void bounce(Asteroid& a, Asteroid& b);

    int destroyAt(sf::Vector2f target);
    
    void draw(sf::RenderTarget& target);
};
