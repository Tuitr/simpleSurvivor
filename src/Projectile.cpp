#include "Projectile.hpp"

#include "DungeonMap.hpp"

Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction, int damage, bool enemyProjectile, float speed, float maxDistance)
    : m_position(position),
      m_direction(normalize(direction)),
      m_speed(speed),
      m_maxDistance(maxDistance),
      m_damage(damage),
      m_enemyProjectile(enemyProjectile)
{
}

void Projectile::update(float dt, const DungeonMap& map)
{
    const float step = m_speed * dt;
    m_position += m_direction * step;
    m_traveled += step;

    if (m_traveled > m_maxDistance || map.isBlocked(getBounds()))
        m_alive = false;
}

void Projectile::draw(sf::RenderWindow& window) const
{
    sf::RectangleShape shaft({18.f, 5.f});
    shaft.setOrigin({9.f, 2.5f});
    shaft.setPosition(m_position);
    shaft.setFillColor(m_enemyProjectile ? sf::Color(117, 72, 177) : sf::Color(151, 103, 58));
    shaft.setRotation(sf::radians(std::atan2(m_direction.y, m_direction.x)));
    window.draw(shaft);

    sf::RectangleShape tip({6.f, 7.f});
    tip.setOrigin({3.f, 3.5f});
    tip.setPosition(m_position + m_direction * 10.f);
    tip.setFillColor(m_enemyProjectile ? sf::Color(205, 128, 244) : sf::Color(220, 220, 210));
    tip.setRotation(sf::radians(std::atan2(m_direction.y, m_direction.x)));
    window.draw(tip);
}

sf::FloatRect Projectile::getBounds() const
{
    return makeRect(m_position, {14.f, 14.f});
}

int Projectile::getDamage() const
{
    return m_damage;
}

bool Projectile::isAlive() const
{
    return m_alive;
}

bool Projectile::isEnemyProjectile() const
{
    return m_enemyProjectile;
}

void Projectile::destroy()
{
    m_alive = false;
}
