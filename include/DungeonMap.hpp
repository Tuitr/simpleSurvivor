#pragma once

#include <SFML/Graphics.hpp>

#include <random>
#include <vector>

class TextureAtlas;

class DungeonMap
{
public:
    DungeonMap();

    void draw(sf::RenderWindow& window, const TextureAtlas* atlas = nullptr) const;
    bool isBlocked(const sf::FloatRect& bounds) const;
    sf::Vector2f randomFreePosition(std::mt19937& rng, sf::Vector2f awayFrom = {0.f, 0.f}) const;
    sf::Vector2f getSize() const;

private:
    sf::Vector2f m_size;
    std::vector<sf::FloatRect> m_walls;
    std::vector<sf::FloatRect> m_decor;

    void drawRect(sf::RenderWindow& window, const sf::FloatRect& rect, sf::Color color) const;
};
