#include "station.hpp"

#include "commons.hpp"


Station::Station(std::shared_ptr<StationSettings>& sSetts):
    m_sSetts(sSetts),
    m_position(0.f, 0.f),
    m_angle(0.f),
    m_radius(sSetts->m_radius),
    m_angVelocity(sSetts->m_angVelocity)
{
    m_repres.setTexture(m_sSetts->m_texture);
    m_repres.setOrigin(sf::Vector2f(m_sSetts->m_texture.getSize()) * 0.5f);
    
    float scale = std::max((float)m_radius / m_sSetts->m_texture.getSize().x,
			   (float)m_radius / m_sSetts->m_texture.getSize().y) * 2.f;
    m_repres.setScale(sf::Vector2f(1.f, 1.f) * scale);
}

void Station::tick()
{
    m_angle = normalizeAngle(m_angle + m_angVelocity);

    m_repres.setRotation(radToDeg(m_angle));
}

void Station::draw(sf::RenderTarget& target)
{
    target.draw(m_repres);
}
