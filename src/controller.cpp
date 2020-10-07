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
    }while(determineLightness(result) < lightRange[0] || determineLightness(result) > lightRange[1]);
    
    return result;
}

Controller::Controller(std::shared_ptr<AsteroidSettings> aSetts, std::shared_ptr<ControllerSettings> cSetts):
    m_aSetts(aSetts), m_cSetts(cSetts), m_lastAstCreated(0)
{
    m_bounds = sf::FloatRect(-(m_cSetts->m_areaWidth /2 +   m_cSetts->m_buffer),
			     -(m_cSetts->m_areaHeight/2 +   m_cSetts->m_buffer),
			       m_cSetts->m_areaWidth    + 2*m_cSetts->m_buffer,
			       m_cSetts->m_areaWidth    + 2*m_cSetts->m_buffer);
}

void Controller::createAsteroid()
{
    int side = randomI(0, 3); // 0 - north, 1 - east, 2 - south, 3 - west
    sf::Vector2f start(0.f, 0.f);
    float angle;
    float targetAngle;

    if(side % 2 == 0)
    {
	start.x = randomI(-(m_cSetts->m_areaWidth/2 + m_cSetts->m_buffer),
			    m_cSetts->m_areaWidth/2 + m_cSetts->m_buffer);
	start.y = (1 - side/2 *2) * (m_cSetts->m_areaHeight/2 + m_cSetts->m_buffer);
    }
    else
    {
	start.x = (1 - side/2 *2) * (m_cSetts->m_areaWidth/2 + m_cSetts->m_buffer);
	start.y = randomI(-(m_cSetts->m_areaHeight/2 + m_cSetts->m_buffer),
			    m_cSetts->m_areaHeight/2 + m_cSetts->m_buffer);
    }

    //targetAngle = atan2();
    angle = normalizeAngle(randomF(0, M_PI) - (side-2) * M_PI/2.f);

    //std::cout << side << ": (" << start.x << ", " << start.y << "|" << angle
    //          << " = {" << cos(angle) << ", " << sin(angle) << "})" << std::endl;
    
    m_asteroids.emplace_back(m_aSetts,
			     randomI(m_cSetts->m_massRange[0], m_cSetts->m_massRange[1]),
			     randomColor(m_cSetts->m_colorLightRange),
			     start,
			     sf::Vector2f(cos(angle), sin(angle)) *
			     randomF(m_cSetts->m_velocityRange[0], m_cSetts->m_velocityRange[1]),
			     randomF(m_cSetts->m_angVelocityRange[0], m_cSetts->m_angVelocityRange[1])
			    );
}

int Controller::tick(int ticksPassed)
{
    std::vector<int> toRemove;
    
    if(ticksPassed - m_lastAstCreated >= m_cSetts->m_period)
    {
	createAsteroid();
	m_lastAstCreated = ticksPassed;
    }
    
    for(int i = 0; i < m_asteroids.size(); ++i)
    {
	if(!m_asteroids[i].tick(m_bounds))
	{
	    toRemove.push_back(i);
	}
    }

    for(int i = toRemove.size()-1; i >= 0; --i)
    {
	m_asteroids.erase(m_asteroids.begin() + toRemove[i]);
    }

    return m_asteroids.size();
}

int Controller::destroyAt(sf::Vector2f target)
{
    int kills = 0;
    std::vector<int> toRemove;
    
    for(int i = 0; i < m_asteroids.size(); ++i)
    {
	if(distance(m_asteroids[i].getPosition(), target) <= m_asteroids[i].getRadius())
	{
	    //std::cout << "GOT 'IM" << i << std::endl;
	    toRemove.push_back(i);
	    ++kills;
	}
    }

    for(int i = toRemove.size()-1; i >= 0; --i)
    {
	m_asteroids.erase(m_asteroids.begin() + toRemove[i]);
    }
    
    return kills;
}

void Controller::draw(sf::RenderTarget& target)
{
    for(int i = 0; i < m_asteroids.size(); ++i)
    {
	m_asteroids[i].draw(target);
    }
}
