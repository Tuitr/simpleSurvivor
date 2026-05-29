#pragma once

#include <SFML/Graphics.hpp>

#include <string>

class Button
{
public:
    Button() = default;
    Button(sf::Vector2f position, sf::Vector2f size, const std::string& label);

    void setPosition(sf::Vector2f position);
    bool contains(sf::Vector2f point) const;
    void draw(sf::RenderWindow& window, const sf::Font* font) const;

private:
    sf::RectangleShape m_shape;
    std::string m_label;
};
