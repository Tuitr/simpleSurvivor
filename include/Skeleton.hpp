#pragma once

#include "Enemy.hpp"

class Skeleton : public Enemy
{
public:
    Skeleton(sf::Vector2f position, int wave, bool armored = false);

    void update(float dt, const sf::Vector2f& playerPosition, const DungeonMap& map) override;
    void draw(sf::RenderWindow& window, const TextureAtlas* atlas = nullptr) const override;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getCenter() const override;
    int getDamage() const override;
    bool canHitPlayer() const override;
    void restartHitCooldown() override;
    void setArmored(bool armored) override;
    bool isArmored() const override;

protected:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    float m_speed = 80.f;
    int m_damage = 8;
    float m_hitCooldown = 0.f;
    int m_strafeSign = 1;
    bool m_armored = false;

    void trySmartMove(sf::Vector2f direction, float distance, const DungeonMap& map);
    bool tryMove(sf::Vector2f delta, const DungeonMap& map);
};

class MageSkeleton : public Skeleton
{
public:
    MageSkeleton(sf::Vector2f position, int wave, bool armored = false);

    void update(float dt, const sf::Vector2f& playerPosition, const DungeonMap& map) override;
    void draw(sf::RenderWindow& window, const TextureAtlas* atlas = nullptr) const override;
    bool isMage() const override;
    bool canShoot() const override;
    Projectile shootAt(sf::Vector2f target) override;

private:
    float m_shootCooldown = 1.4f;
};
