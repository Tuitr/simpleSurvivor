#include "Enemy.hpp"

void Enemy::takeDamage(int damage)
{
    m_health -= damage;
}

bool Enemy::isDead() const
{
    return m_health <= 0;
}

float Enemy::getHealthRatio() const
{
    if (m_maxHealth <= 0)
        return 0.f;

    return static_cast<float>(m_health) / static_cast<float>(m_maxHealth);
}

void Enemy::setArmored(bool)
{
}

bool Enemy::isArmored() const
{
    return false;
}

bool Enemy::isMage() const
{
    return false;
}

bool Enemy::canShoot() const
{
    return false;
}

Projectile Enemy::shootAt(sf::Vector2f target)
{
    return Projectile(target, {0.f, 1.f}, 0, true);
}
