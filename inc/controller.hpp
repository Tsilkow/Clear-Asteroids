#pragma once

#include <iostream>
#include <vector>
#include "asteroids.hpp"


struct ControllerSettings
{
    int m_frequency;
    float m_bounce;
};

class Controller
{
    private:
    std::vector<Asteroid> m_asteroids;
    std::shared_ptr<AsteroidSettings> m_aSetts;
    std::shared_ptr<ControllerSettings> m_cSetts;

    void createAsteroid();
    
    public:
    Controller(std::shared_ptr<AsteroidSettings> aSetts, std::shared_ptr<ControllerSettings> cSetts);

    void tick(int ticksPassed);
};
