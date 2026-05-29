#pragma once

#include <SFML/Graphics.hpp>

class TextureAtlas;

enum class WeaponUpgradeType
{
    Damage,
    AttackSpeed
};

class WeaponBooster
{
public:
    WeaponBooster(sf::Vector2f position, WeaponUpgradeType type);

    void draw(sf::RenderWindow& window, const TextureAtlas* atlas = nullptr) const;
    sf::FloatRect getBounds() const;
    WeaponUpgradeType getType() const;

private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    WeaponUpgradeType m_type;
};
