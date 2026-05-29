#pragma once

#include <SFML/Graphics.hpp>

class TextureAtlas;

class HealthPack
{
public:
    explicit HealthPack(sf::Vector2f position);

    void draw(sf::RenderWindow& window, const TextureAtlas* atlas = nullptr) const;
    sf::FloatRect getBounds() const;
    int getHealAmount() const;

private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    int m_healAmount = 28;
};
