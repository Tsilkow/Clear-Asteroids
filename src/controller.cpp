#include "controller.hpp"


#include "randoms.hpp"
#include <math.h>


float determineLightness(sf::Color input)
{
    // https://thoughtbot.com/blog/closer-look-color-lightness
    // values have been normalized (ie divided by 255)
    return ((float)input.r) * 0.00083372549 +
	   ((float)input.g) * 0.00280470588 +
	   ((float)input.b) * 0.00028313725;
}

sf::Color randomColor(std::vector<float> lightRange)
{
    sf::Color result(0, 0, 0);
    do{
	result.r = randomI(0, 255);
	result.g = randomI(0, 255);
	result.b = randomI(0, 255);
    }while(determineLightness(result) > lightRange[0] && determineLightness(result) < lightRange[1]);

    return result;
}

Controller::Controller(std::shared_ptr<AsteroidSettings> aSetts, std::shared_ptr<ControllerSettings> cSetts):
    m_aSetts(aSetts), m_cSetts(cSetts), m_lastAstCreated(0)
{
    ;
}

void Controller::createAsteroid()
{
    int side = randomI(0, 3); // 0 - north, 1 - east, 2 - south, 3 - west
    sf::Vector2f start(0.f, 0.f);
    float angle = normalizeAngle(randomF(M_PI, 2*M_PI) - side * M_PI/2.f);

    if(side % 2 == 0)
    {
	start.x = randomI(-(m_cSetts->m_areaWidth/2 + m_cSetts->m_buffer),
			    m_cSetts->m_areaWidth/2 + m_cSetts->m_buffer);
	start.y = (side/2 *2 - 1) * (m_cSetts->m_areaHeight/2 + m_cSetts->m_buffer);
    }
    else
    {
	start.x = (side/2 *2 - 1) * (m_cSetts->m_areaWidth/2 + m_cSetts->m_buffer);
	start.y = randomI(-(m_cSetts->m_areaHeight/2 + m_cSetts->m_buffer),
			    m_cSetts->m_areaHeight/2 + m_cSetts->m_buffer);
    }

    //std::cout << side << ": (" << start.x << ", " << start.y << "|" << angle << ")" << std::endl;
    
    m_asteroids.emplace_back(m_aSetts,
			     randomI(m_cSetts->m_massRange[0], m_cSetts->m_massRange[1]),
			     randomColor(m_cSetts->m_colorLightRange),
			     start,
			     sf::Vector2f(-cos(angle), -sin(angle)) *
			     randomF(m_cSetts->m_velocityRange[0], m_cSetts->m_velocityRange[1]),
			     randomF(m_cSetts->m_angVelocityRange[0], m_cSetts->m_angVelocityRange[1])
			    );
}

void Controller::tick(int ticksPassed)
{
    if(ticksPassed - m_lastAstCreated >= m_cSetts->m_period)
    {
	createAsteroid();
	m_lastAstCreated = ticksPassed;
    }
    
    for(int i = 0; i < m_asteroids.size(); ++i)
    {
	m_asteroids[i].tick();
    }
}

void Controller::draw(sf::RenderTarget& target)
{
    for(int i = 0; i < m_asteroids.size(); ++i)
    {
	m_asteroids[i].draw(target);
    }
}
