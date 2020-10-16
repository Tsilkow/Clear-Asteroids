#include "controller.hpp"

#include "commons.hpp"
#include <math.h>



Controller::Controller(std::shared_ptr<AsteroidSettings>& aSetts, std::shared_ptr<ControllerSettings>& cSetts,
		       std::shared_ptr<StationSettings>& sSetts):
    m_aSetts(aSetts),
    m_cSetts(cSetts),
    m_sSetts(sSetts),
    m_lastAstCreated(0)
{
    m_APM = (float)m_cSetts->m_startAPM;
    
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

    // an angle from start of the asteroid to the center of screen
    targetAngle = atan2(-start.y, -start.x);
    if(targetAngle <   M_PI - side * M_PI/2.f) targetAngle += 2 * M_PI;
    if(targetAngle > 2*M_PI - side * M_PI/2.f) targetAngle -= 2 * M_PI;
    angle = normalizeAngle(randomFwT(M_PI - side * M_PI/2.f, 2*M_PI - side * M_PI/2.f,
				     targetAngle, m_cSetts->m_targetting));

    //std::cout << side << ": (" << start.x << ", " << start.y << "|" << targetAngle
    //          << " = {" << cos(targetAngle) << ", " << sin(targetAngle) << "})" << std::endl;
    
    m_asteroids.emplace_back(m_aSetts,
			     randomI(m_cSetts->m_massRange[0], m_cSetts->m_massRange[1]),
			     randomColor(m_cSetts->m_colorLightRange),
			     start,
			     sf::Vector2f(cos(angle), sin(angle)) *
			     randomF(m_cSetts->m_velocityRange[0], m_cSetts->m_velocityRange[1]),
			     randomF(m_cSetts->m_angVelocityRange[0], m_cSetts->m_angVelocityRange[1])
			    );
}

bool Controller::tick(bool action, int ticksPassed)
{
    std::vector<int> toRemove;
    
    if(ticksPassed - m_lastAstCreated >= 3600.f/m_APM)
    {
	createAsteroid();
	m_lastAstCreated = ticksPassed;
    }

    m_APM += ((float)m_cSetts->m_APMincrease) / 3600.f;
    
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
    
    for(int i = 0; i < m_asteroids.size(); ++i)
    {
	for(int j = i+1; j < m_asteroids.size(); ++j)
	{
	    if(distance(m_asteroids[i].getPosition(), m_asteroids[j].getPosition())
	       <= m_asteroids[i].getRadius() + m_asteroids[j].getRadius())
	    {
		bounce(m_asteroids[i], m_asteroids[j]);
	    }
	}
    }

    if(m_station)
    {
	m_station->tick();
	for(int i = 0; i < m_asteroids.size(); ++i)
	{
	    if(distance(m_asteroids[i].getPosition(), m_station->getPosition())
	       <= m_asteroids[i].getRadius() + m_station->getRadius())
	    {
		return false;
	    }
	}
    }

    return true;
}

void Controller::bounce(Asteroid& a, Asteroid& b)
{
    /*float m1 = a.getMass();
    float m2 = b.getMass();
    sf::Vector2f v1 = a.getVelocity();
    sf::Vector2f v2 = b.getVelocity();
    float bounce = m_cSetts->m_bounce;
    sf::Vector2f force  = (m1 * (1 - bounce) - m2 * (1 + bounce))/((m1 + m2) * bounce) * v1 * m1 +
	                  (m2 * (bounce + 1))/((m1 + m2) * bounce) * v2 * m1;
    sf::Vector2f force2 = (m2 * (1 - bounce) - m1 * (1 + bounce))/((m1 + m2) * bounce) * v2 * m2 +
                          (m1 * (bounce + 1))/((m1 + m2) * bounce) * v1 * m2;
     
    std::cout << "(" << force.x << ", " << force.y << ")" << std::endl;
    std::cout << "(" << force2.x << ", " << force2.y << ")" << std::endl;*/

    sf::Vector2f connector = b.getPosition() - a.getPosition();
    float alongDir = dotProduct(b.getVelocity() - a.getVelocity(), connector/length(connector));
	    
    if(alongDir <= 0)
    {
	sf::Vector2f impulse = -alongDir * connector/length(connector)/(1.f/a.getMass() + 1.f/b.getMass());

	a.applyForce(-impulse * (1 + m_cSetts->m_bounce));
	b.applyForce( impulse * (1 + m_cSetts->m_bounce));
    }
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

void Controller::start()
{
    m_station = std::make_shared<Station>(m_sSetts);
    reset();
}

void Controller::reset()
{
    m_APM = (float)m_cSetts->m_startAPM;
}

void Controller::killStation()
{
    m_station = nullptr;
}

void Controller::draw(sf::RenderTarget& target)
{
    for(int i = 0; i < m_asteroids.size(); ++i)
    {
	m_asteroids[i].draw(target);
    }
    if(m_station) m_station->draw(target);
}
