#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


struct AsteroidSettings
{
    float m_invDensity; // inverse of density
    int m_TOSegments;
    float m_amplitude;
    int m_peakFrequency;
    int m_peakVariation;
    float m_peakAmplitude;
};


// generation of smooth and semi-random change of heights between two set points
void connectPeaks(std::vector<float>& result, int begin, int end, float amplitude, int loop);

// generation of wrapping landscape of heigts
std::vector<float> generateHeights(int TOSegments, float amplitude, int peakFrequency, int peakVariation, float peakAmplitude);

class Asteroid
{
    private:
    int m_mass;
    sf::Color m_color;
    sf::Vector2f m_position;
    float m_angle;
    sf::Vector2f m_velocity;
    float m_angVelocity;
    sf::Vector2f m_force;

    int m_radius;
    std::vector<sf::Vertex> m_staRepres; // static representation
    std::vector<sf::Vertex> m_dynRepres; // dynamic representation
    sf::CircleShape m_collisionMask;
    
    public:
    Asteroid(const std::shared_ptr<AsteroidSettings> aSetts, int mass, sf::Color color, sf::Vector2f position, sf::Vector2f velocity, float angVelocity);

    void applyForce(sf::Vector2f force) {m_force += force; }

    bool tick(sf::FloatRect bounds);

    void draw(sf::RenderTarget& target);

    const sf::Vector2f getPosition() {return m_position; }
    const sf::Vector2f getVelocity() {return m_velocity; }
    const int getMass()   {return m_mass;   }
    const int getRadius() {return m_radius; }
};
