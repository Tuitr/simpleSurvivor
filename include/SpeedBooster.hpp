#pragma once

#include <SFML/Graphics.hpp>

class TextureAtlas;

class SpeedBooster
{
public:
    explicit SpeedBooster(sf::Vector2f position);

    void draw(sf::RenderWindow& window, const TextureAtlas* atlas = nullptr) const;
    sf::FloatRect getBounds() const;
    float getDuration() const;
    float getMultiplier() const;

private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    float m_duration = 5.f;
    float m_multiplier = 1.45f;
};
