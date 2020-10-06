#include "crosshair.hpp"


#include <math.h>

float length(sf::Vector2f input)
{
    return sqrt(input.x*input.x + input.y*input.y);
}

float distance(sf::Vector2f a, sf::Vector2f b)
{
    return length(a - b);
}

void gradientColor(std::vector<sf::Color>& result, sf::Color start, sf::Color end, int steps)
{
    result.emplace_back(start);
    for(int i = 1; i < steps; ++i)
    {
	result.emplace_back((int)std::round((float)start.r + (float)(end.r - start.r) * (float)i/(float)steps),
			    (int)std::round((float)start.g + (float)(end.g - start.g) * (float)i/(float)steps),
			    (int)std::round((float)start.b + (float)(end.b - start.b) * (float)i/(float)steps));
    }
    result.emplace_back(end);
}

Crosshair::Crosshair(std::shared_ptr<CrosshairSettings>& crSetts):
    m_crSetts(crSetts),
    m_position(0.f, 0.f),
    m_lastShot(0)
{
    for(int i = 0; i < m_crSetts->m_shape.size(); ++i)
    {
	m_dynRepres.emplace_back(m_crSetts->m_shape[i] + m_position, m_color);
    }
    gradientColor(m_gradient, m_crSetts->m_coolColor, m_crSetts->m_normColor, m_crSetts->m_cooldown);
    m_color = m_crSetts->m_coolColor;
}

void Crosshair::tick(int ticksPassed, sf::Vector2f mouse)
{
    if(distance(mouse, m_position) <= (float)m_crSetts->m_maneuverability)
    {
	m_position = mouse;
    }
    else
    {
	m_position = m_position + (mouse - m_position) *
	    (float)m_crSetts->m_maneuverability / distance(mouse, m_position);
    }

    if(ticksPassed - m_lastShot <= m_crSetts->m_cooldown)
    {
	m_color = m_gradient[ticksPassed - m_lastShot];
    }

    for(int i = 0; i < m_crSetts->m_shape.size(); ++i)
    {
	m_dynRepres[i].position = m_crSetts->m_shape[i] + m_position;
	m_dynRepres[i].color = m_color;
    }
}

bool Crosshair::shoot(int ticksPassed)
{
    if(ticksPassed - m_lastShot >= m_crSetts->m_cooldown)
    {
	std::cout << "(" << m_position.x << " " << m_position.y << ")" << std::endl;
	m_lastShot = ticksPassed;
	return true;
    }
    return false;
}

void Crosshair::draw(sf::RenderTarget& target)
{
    target.draw(&m_dynRepres[0], m_dynRepres.size(), sf::Lines);
}
