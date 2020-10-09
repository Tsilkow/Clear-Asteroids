#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


struct StationSettings
{
    int m_radius;
    float m_angVelocity;
    sf::Texture m_texture;
};

class Station
{
    private:
    std::shared_ptr<StationSettings> m_sSetts;
    sf::Vector2f m_position;
    float m_angle;
    int m_radius;
    float m_angVelocity;
    sf::Sprite m_repres;
    sf::CircleShape m_mask;

    public:
    Station(std::shared_ptr<StationSettings>& sSetts);

    void tick();

    void draw(sf::RenderTarget& target);

    const sf::Vector2f getPosition() {return m_position; }
    const int getRadius() {return m_radius; }
};
