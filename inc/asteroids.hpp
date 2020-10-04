#pragma once

#include <iostream>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

struct AsteroidSettings
{
    float m_density;
    std::vector<float> m_velocityRange;
    std::vector<int> m_massRange;
    std::vector<float> m_angVelocityRange; //angular velocity
    int TOSegments;
    std::vector<float> m_colorLightRange;
    float m_amplitude;
    int m_peakFrequency;
    int m_peakVariation;
    int m_peakAmplitude;
};

void generateShape(std::vector<Vertex> result, const std::shared_ptr<AsteroidSettings> aSetts, int mass, sf::Color color);

class Asteroid
{
    private:
    std::shared_ptr<AsteroidSettings> m_aSetts;
    int m_mass;
    sf::Color m_color;
    sf::Vector2f m_position;
    float m_angle;
    sf::Vector2f m_velocity;
    float m_angVelocity;

    std::vector<sf::Vertex> m_staRepres; // static representation
    std::vector<sf::Vertex> m_dynRepres; // dynamic representation
    
    public:
    Asteroid(const std::shared_ptr<AsteroidSettings> aSetts, int mass, sf::Color color, sf::Vector2f position, sf::Vector2f velocity, float angVelocity);

    void tick();

    void draw(sf::RenderTarget& target);
};
