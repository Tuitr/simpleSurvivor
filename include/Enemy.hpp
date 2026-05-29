#pragma once

#include "Common.hpp"
#include "Projectile.hpp"

class DungeonMap;
class TextureAtlas;

class Enemy
{
public:
    virtual ~Enemy() = default;

    virtual void update(float dt, const sf::Vector2f& playerPosition, const DungeonMap& map) = 0;
    virtual void draw(sf::RenderWindow& window, const TextureAtlas* atlas = nullptr) const = 0;
    virtual sf::FloatRect getBounds() const = 0;
    virtual sf::Vector2f getCenter() const = 0;
    virtual int getDamage() const = 0;
    virtual bool canHitPlayer() const = 0;
    virtual void restartHitCooldown() = 0;
    virtual void setArmored(bool armored);
    virtual bool isArmored() const;
    virtual bool isMage() const;
    virtual bool canShoot() const;
    virtual Projectile shootAt(sf::Vector2f target);

    void takeDamage(int damage);
    bool isDead() const;
    float getHealthRatio() const;

protected:
    int m_health = 1;
    int m_maxHealth = 1;
};
