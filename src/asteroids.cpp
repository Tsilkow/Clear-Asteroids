#include "asteroids.hpp"


void generateShape(std::vector<Vertex> result, const std::shared_ptr<AsteroidSettings> aSetts, int mass, sf::Color color)
{
    std::cout << "BABABOUI" << std::endl;
}

Asteroid::Asteroid(const std::shared_ptr<AsteroidSettings> aSetts, int mass, sf::Color color, sf::Vector2f postion, sf::Vector2f velocity, float angVelocity):
    m_aSetts(aSetts),
    m_mass(mass),
    m_color(color),
    m_position(position),
    m_angle(0.f),
    m_velocity(velocity),
    m_angVelocity(angVelocity)
{
    generateShape(m_staRepres, m_aSetts, mass, color);
}

void Asteroid::tick()
{
    m_velocity += m_force * 1.f/m_mass;
    m_force = sf::Vector2f(0.f, 0.f);
    m_position += m_velocity;
}

void Asteroid::draw(sf::RenderTarget& target);
{
    target.draw(&m_dynRepres[0], m_dynRepres.size(), sf::TriangleFan);
}


