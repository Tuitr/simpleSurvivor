#pragma once

#include <SFML/Graphics.hpp>

#include <cmath>
#include <string>

enum class GameState
{
    MainMenu,
    Playing,
    GameOver
};

enum class WeaponType
{
    Sword,
    Bow
};

inline float vectorLength(sf::Vector2f value)
{
    return std::sqrt(value.x * value.x + value.y * value.y);
}

inline sf::Vector2f normalize(sf::Vector2f value)
{
    const float length = vectorLength(value);
    if (length <= 0.001f)
        return {0.f, -1.f};

    return {value.x / length, value.y / length};
}

inline sf::FloatRect makeRect(sf::Vector2f center, sf::Vector2f size)
{
    return {{center.x - size.x * 0.5f, center.y - size.y * 0.5f}, size};
}

inline bool intersects(const sf::FloatRect& a, const sf::FloatRect& b)
{
    return a.findIntersection(b).has_value();
}

std::string formatTime(float seconds);
