#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


struct CrosshairSettings
{
    int m_cooldown;
    int m_maneuverability;
    std::vector<sf::Vector2f> m_shape;
    sf::Color m_normColor;
    sf::Color m_coolColor;
};

float length(sf::Vector2f input);

float distance(sf::Vector2f a, sf::Vector2f b);

class Crosshair
{
    private:
    std::shared_ptr<CrosshairSettings> m_crSetts;
    sf::Vector2f m_position;
    int m_lastShot;
    std::vector<sf::Vertex> m_dynRepres; // dynamic representation
    sf::Color m_color;
    std::vector<sf::Color> m_gradient;

    public:
    Crosshair(std::shared_ptr<CrosshairSettings>& crSetts);

    void tick(int ticksPassed, sf::Vector2f mouse);

    bool shoot(int ticksPassed);

    void draw(sf::RenderTarget& target);

    const sf::Vector2f getPosition() {return m_position; }
};
