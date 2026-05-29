#pragma once

#include "Common.hpp"
#include "Weapon.hpp"

#include <memory>

class DungeonMap;
class TextureAtlas;

class Player
{
public:
    Player();

    void reset(sf::Vector2f position);
    void handleInput(float dt, const DungeonMap& map);
    void update(float dt);
    void draw(sf::RenderWindow& window, const TextureAtlas* atlas = nullptr) const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getCenter() const;
    sf::Vector2f getFacing() const;
    void lookAt(sf::Vector2f worldPosition);

    void switchWeapon();
    WeaponType getWeaponType() const;
    std::string getWeaponName() const;
    Weapon& getWeapon();
    const Weapon& getWeapon() const;
    bool canAttack() const;
    void restartAttackCooldown();

    void takeDamage(int damage);
    void heal(int amount);
    void applySpeedBoost(float seconds, float multiplier);
    void upgradeWeaponDamage();
    void upgradeAttackSpeed();
    int getWeaponDamageLevel() const;
    int getAttackSpeedLevel() const;
    float getWeaponDamageMultiplier() const;
    float getAttackCooldownMultiplier() const;
    bool isDead() const;
    int getHealth() const;
    int getMaxHealth() const;
    float getSpeedBoostLeft() const;

private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Vector2f m_facing;
    int m_health = 100;
    int m_maxHealth = 100;
    float m_baseSpeed = 180.f;
    float m_speed = 180.f;
    float m_speedBoostTimer = 0.f;
    float m_speedBoostMultiplier = 1.f;
    float m_attackTimer = 0.f;
    int m_weaponDamageLevel = 0;
    int m_attackSpeedLevel = 0;
    WeaponType m_selectedWeapon = WeaponType::Sword;
    std::unique_ptr<Weapon> m_weapon;

    void setWeapon(WeaponType type);
    void tryMove(sf::Vector2f delta, const DungeonMap& map);
};
