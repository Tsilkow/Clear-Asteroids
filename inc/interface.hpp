#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


class Interface
{
    private:
    sf::Font m_font;
    std::vector<sf::Text> m_strings;
    std::vector<sf::Vector2f> m_positions;
    std::vector<int> m_alignments;

    public:
    Interface(sf::Font& font);

    void addString(std::string content, sf::Vector2f position, int alignment = 0, int size = 24,
		   sf::Color color = sf::Color(255, 255, 255));

    bool adjustPosition(int index);

    bool setContent(int index, std::string content);

    bool setPosition(int index, sf::Vector2f position);

    // -1 is left, 0 is middle, 1 is right
    bool setAlignment(int index, int alignment);

    bool setSize(int index, int size);

    bool setStyle(int index, int style);

    bool setTColor(int index, sf::Color color);

    void tick(int ticksPassed);

    void draw(sf::RenderTarget& target);
};