#pragma once

#include "Common.hpp"

class DungeonMap;

class Projectile
{
public:
    Projectile(sf::Vector2f position, sf::Vector2f direction, int damage, bool enemyProjectile = false, float speed = 520.f, float maxDistance = 620.f);

    void update(float dt, const DungeonMap& map);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    int getDamage() const;
    bool isAlive() const;
    bool isEnemyProjectile() const;
    void destroy();

private:
    sf::Vector2f m_position;
    sf::Vector2f m_direction;
    float m_speed = 520.f;
    float m_traveled = 0.f;
    float m_maxDistance = 620.f;
    int m_damage = 18;
    bool m_enemyProjectile = false;
    bool m_alive = true;
};
