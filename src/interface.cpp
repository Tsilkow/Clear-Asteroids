#include "interface.hpp"

#include <math.h>


Interface::Interface(sf::Font& font):
    m_font(font)
{
    ;
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
	m_strings[index].setStyle(style);
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
		     
void Interface::tick(int ticksPassed)
{
    ;
}

void Interface::draw(sf::RenderTarget& target)
{
    for(int i = 0 ; i < m_strings.size(); ++i)
    {
	target.draw(m_strings[i]);
    }
}
