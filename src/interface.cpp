#include "interface.hpp"


Interface::Interface(sf::Font& font):
    m_font(font)
{
    ;
}

void Interface::addString(std::string content, sf::Vector2f position, int size, sf::Color color)
{
    m_strings.push_back(sf::Text(content, m_font, size));
    setPosition(-1, position);
    setTColor(-1, color);
}

bool Interface::setContent(int index, std::string content)
{
    if(index == -1) index = m_strings.size()-1;
    if(index < m_strings.size())
    {
	m_strings[index].setString(content);
	return true;
    }
    return false;
}

bool Interface::setPosition(int index, sf::Vector2f position)
{
    if(index == -1) index = m_strings.size()-1;
    if(index < m_strings.size())
    {
	m_strings[index].setPosition(position);
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
