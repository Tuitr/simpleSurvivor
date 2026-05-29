#pragma once

#include "Common.hpp"
#include "Projectile.hpp"

#include <memory>

class Weapon
{
public:
    virtual ~Weapon() = default;
    virtual WeaponType type() const = 0;
    virtual std::string name() const = 0;
    virtual float cooldown() const = 0;
};

class Sword : public Weapon
{
public:
    WeaponType type() const override;
    std::string name() const override;
    float cooldown() const override;
    int damage() const;
    sf::FloatRect hitBox(sf::Vector2f playerCenter, sf::Vector2f facing) const;
    void drawHitBox(sf::RenderWindow& window, const sf::FloatRect& hitBox) const;
    void drawSwing(sf::RenderWindow& window, sf::Vector2f playerCenter, sf::Vector2f facing) const;
};

class Bow : public Weapon
{
public:
    WeaponType type() const override;
    std::string name() const override;
    float cooldown() const override;
    Projectile shoot(sf::Vector2f playerCenter, sf::Vector2f facing) const;
    Projectile shoot(sf::Vector2f playerCenter, sf::Vector2f facing, int damage) const;
    int damage() const;
};
