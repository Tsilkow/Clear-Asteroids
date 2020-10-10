#include "interface.hpp"

#include <math.h>


Button::Button(sf::Text string, sf::FloatRect bounds):
    m_string(string), m_bounds(bounds)
{
    m_string.setFillColor(sf::Color::White);
    m_string.setOutlineColor(sf::Color::White);
    m_string.setOutlineThickness(0);
}

bool Button::tick(sf::Vector2f mouse)
{
    if(m_bounds.contains(mouse))
    {
	m_string.setFillColor(sf::Color::Black);
	m_string.setOutlineThickness(3);
	return true;
    }
    m_string.setFillColor(sf::Color::White);
    m_string.setOutlineThickness(0);
    return false;
}

void Button::draw(sf::RenderTarget& target)
{
    target.draw(m_string);
}

Interface::Interface(sf::Font& font, sf::Vector2f size, sf::Color backdropColor):
    m_font(font)
{
    m_backdrop.setPosition(-size * 0.5f);
    m_backdrop.setSize(size);
    m_backdrop.setFillColor(backdropColor);
}

void Interface::addString(std::string content, sf::Vector2f position, int alignment, int size, sf::Color color)
{
    m_strings.push_back(sf::Text(content, m_font, size));
    m_positions.push_back(position);
    m_alignments.push_back(alignment);
    setPosition(-1, position);
    setTColor(-1, color);
    adjustPosition(-1);
}

void Interface::addButton(int index, sf::FloatRect bounds)
{
    if(index == -1) index = m_strings.size()-1;
    if(bounds.width == 0 || bounds.height == 0) bounds = m_strings[index].getGlobalBounds();
    m_buttons.emplace_back(m_strings[index], bounds);
    m_strings.erase(m_strings.begin() + index);
}

bool Interface::adjustPosition(int index)
{
    if(index == -1) index = m_strings.size()-1;
    if(index < m_strings.size())
    {
	float width = m_strings[index].getGlobalBounds().width;

	m_strings[index].setPosition(
	       sf::Vector2f(
		      std::round(m_positions[index].x - (m_alignments[index]+1) * width * 0.5f),
		                 m_positions[index].y));
	return true;
    }
    return false;
}

bool Interface::setContent(int index, std::string content)
{
    if(index == -1) index = m_strings.size()-1;
    if(index < m_strings.size())
    {
	m_strings[index].setString(content);
	adjustPosition(index);
	return true;
    }
    return false;
}

bool Interface::setPosition(int index, sf::Vector2f position)
{
    if(index == -1) index = m_strings.size()-1;
    if(index < m_strings.size())
    {
	m_positions[index] = position;
	m_strings[index].setPosition(position);
	adjustPosition(index);
	return true;
    }
    return false;
}

bool Interface::setAlignment(int index, int alignment)
{
    if(index == -1) index = m_strings.size()-1;
    if(index < m_strings.size())
    {
	m_alignments[index] = alignment;
	adjustPosition(index);
	return true;
    }
    return false;
}

bool Interface::setSize(int index, int size)
{
    if(index == -1) index = m_strings.size()-1;
    if(index < m_strings.size())
    {
	m_strings[index].setCharacterSize(size);
	adjustPosition(index);
	return true;
    }
    return false;
}

bool Interface::setStyle(int index, int style)
{
    if(index == -1) index = m_strings.size()-1;
    if(index < m_strings.size())
    {
	switch(style)
	{
	    case 0: m_strings[index].setStyle(sf::Text::Style::Regular); break;
	    case 1: m_strings[index].setStyle(sf::Text::Style::Bold); break;
	    case 2: m_strings[index].setStyle(sf::Text::Style::Italic); break;
	    case 3: m_strings[index].setStyle(sf::Text::Style::Underlined); break;
	    case 4: m_strings[index].setStyle(sf::Text::Style::StrikeThrough); break;
	}
	return true;
    }
    return false;
}

bool Interface::setTColor(int index, sf::Color color)
{
    if(index == -1) index = m_strings.size()-1;
    if(index < m_strings.size())
    {
	m_strings[index].setColor(color);
	return true;
    }
    return false;
}
		     
int Interface::tick(sf::Vector2f mouse)
{
    for(int i = 0; i < m_buttons.size(); ++i)
    {
	if(m_buttons[i].tick(mouse)) return i;
    }
    return -1;
}

void Interface::draw(sf::RenderTarget& target)
{
    target.draw(m_backdrop);
    for(int i = 0 ; i < m_strings.size(); ++i)
    {
	target.draw(m_strings[i]);
    }
    for(int i = 0 ; i < m_buttons.size(); ++i)
    {
        m_buttons[i].draw(target);
    }
}
