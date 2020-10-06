#pragma once

#include <iostream>
#include <vector>
#include "asteroid.hpp"
#include "crosshair.hpp"


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

    // asteroid simulation settings
    int m_period; // time between creating
    float m_bounce;
};

class Controller
{
    private:
    std::vector<Asteroid> m_asteroids;
    std::shared_ptr<AsteroidSettings> m_aSetts;
    std::shared_ptr<ControllerSettings> m_cSetts;
    int m_lastAstCreated; // tick when last asteroid was created
    sf::FloatRect m_bounds;

    void createAsteroid();
    
    public:
    Controller(std::shared_ptr<AsteroidSettings> aSetts, std::shared_ptr<ControllerSettings> cSetts);

    int tick(int ticksPassed);

    int destroyAt(sf::Vector2f target);
    
    void draw(sf::RenderTarget& target);
};
