#include "Player.hpp"

#include "DungeonMap.hpp"
#include "TextureAtlas.hpp"

#include <algorithm>

Player::Player()
    : m_position(1280.f, 900.f),
      m_size(30.f, 34.f),
      m_facing(0.f, -1.f)
{
    setWeapon(WeaponType::Sword);
}

void Player::reset(sf::Vector2f position)
{
    m_position = {position.x - m_size.x * 0.5f, position.y - m_size.y * 0.5f};
    m_facing = {0.f, -1.f};
    m_health = m_maxHealth;
    m_attackTimer = 0.f;
    m_weaponDamageLevel = 0;
    m_attackSpeedLevel = 0;
    m_speedBoostTimer = 0.f;
    m_speedBoostMultiplier = 1.f;
    m_speed = m_baseSpeed;
    m_selectedWeapon = WeaponType::Sword;
    setWeapon(m_selectedWeapon);
}

void Player::handleInput(float dt, const DungeonMap& map)
{
    sf::Vector2f move{0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        move.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        move.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        move.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        move.x += 1.f;

    if (vectorLength(move) > 0.01f)
    {
        move = normalize(move);
        tryMove({move.x * m_speed * dt, 0.f}, map);
        tryMove({0.f, move.y * m_speed * dt}, map);
    }
}

void Player::update(float dt)
{
    m_attackTimer -= dt;
    m_speedBoostTimer -= dt;

    if (m_speedBoostTimer > 0.f)
        m_speed = m_baseSpeed * m_speedBoostMultiplier;
    else
    {
        m_speedBoostTimer = 0.f;
        m_speedBoostMultiplier = 1.f;
        m_speed = m_baseSpeed;
    }
}

void Player::draw(sf::RenderWindow& window, const TextureAtlas* atlas) const
{
    if (atlas && atlas->isLoaded())
    {
        SpriteId id = SpriteId::KnightDown;
        if (std::abs(m_facing.x) > std::abs(m_facing.y))
            id = m_facing.x < 0.f ? SpriteId::KnightLeft : SpriteId::KnightRight;
        else
            id = m_facing.y < 0.f ? SpriteId::KnightUp : SpriteId::KnightDown;

        atlas->drawSprite(window, id, getCenter() + sf::Vector2f{0.f, -10.f}, {64.f, 82.f});

        if (m_speedBoostTimer > 0.f)
        {
            sf::RectangleShape glow({40.f, 8.f});
            glow.setOrigin({20.f, 4.f});
            glow.setPosition({getCenter().x, getCenter().y + 22.f});
            glow.setFillColor(sf::Color(73, 205, 173, 140));
            window.draw(glow);
        }

        return;
    }

    sf::RectangleShape shadow({34.f, 10.f});
    shadow.setPosition({m_position.x - 2.f, m_position.y + 27.f});
    shadow.setFillColor(sf::Color(15, 15, 20, 140));
    window.draw(shadow);

    sf::RectangleShape legs({24.f, 10.f});
    legs.setPosition({m_position.x + 3.f, m_position.y + 24.f});
    legs.setFillColor(sf::Color(66, 74, 92));
    window.draw(legs);

    sf::RectangleShape armor({28.f, 22.f});
    armor.setPosition({m_position.x + 1.f, m_position.y + 7.f});
    armor.setFillColor(sf::Color(136, 154, 174));
    window.draw(armor);

    sf::RectangleShape helmet({22.f, 14.f});
    helmet.setPosition({m_position.x + 4.f, m_position.y});
    helmet.setFillColor(sf::Color(180, 192, 202));
    window.draw(helmet);

    sf::RectangleShape plume({8.f, 7.f});
    plume.setPosition({m_position.x + 11.f, m_position.y - 5.f});
    plume.setFillColor(sf::Color(167, 51, 61));
    window.draw(plume);

    sf::RectangleShape shield({7.f, 16.f});
    shield.setPosition({m_position.x - 5.f, m_position.y + 12.f});
    shield.setFillColor(sf::Color(69, 96, 160));
    window.draw(shield);

    sf::RectangleShape facingPixel({6.f, 6.f});
    facingPixel.setPosition(getCenter() + m_facing * 19.f - sf::Vector2f{3.f, 3.f});
    facingPixel.setFillColor(sf::Color(245, 226, 135));
    window.draw(facingPixel);

    if (m_speedBoostTimer > 0.f)
    {
        sf::RectangleShape boot({8.f, 5.f});
        boot.setFillColor(sf::Color(73, 205, 173));
        boot.setPosition({m_position.x + 2.f, m_position.y + 32.f});
        window.draw(boot);
        boot.setPosition({m_position.x + 20.f, m_position.y + 32.f});
        window.draw(boot);
    }
}

sf::FloatRect Player::getBounds() const
{
    return {m_position, m_size};
}

sf::Vector2f Player::getCenter() const
{
    return {m_position.x + m_size.x * 0.5f, m_position.y + m_size.y * 0.5f};
}

sf::Vector2f Player::getFacing() const
{
    return m_facing;
}

void Player::lookAt(sf::Vector2f worldPosition)
{
    m_facing = normalize(worldPosition - getCenter());
}

void Player::switchWeapon()
{
    m_selectedWeapon = (m_selectedWeapon == WeaponType::Sword) ? WeaponType::Bow : WeaponType::Sword;
    setWeapon(m_selectedWeapon);
}

WeaponType Player::getWeaponType() const
{
    return m_selectedWeapon;
}

std::string Player::getWeaponName() const
{
    return m_weapon ? m_weapon->name() : "None";
}

Weapon& Player::getWeapon()
{
    return *m_weapon;
}

const Weapon& Player::getWeapon() const
{
    return *m_weapon;
}

bool Player::canAttack() const
{
    return m_attackTimer <= 0.f;
}

void Player::restartAttackCooldown()
{
    m_attackTimer = m_weapon->cooldown() * getAttackCooldownMultiplier();
}

void Player::takeDamage(int damage)
{
    m_health = std::max(0, m_health - damage);
}

void Player::heal(int amount)
{
    m_health = std::min(m_maxHealth, m_health + amount);
}

void Player::applySpeedBoost(float seconds, float multiplier)
{
    m_speedBoostTimer = seconds;
    m_speedBoostMultiplier = multiplier;
}

void Player::upgradeWeaponDamage()
{
    ++m_weaponDamageLevel;
}

void Player::upgradeAttackSpeed()
{
    ++m_attackSpeedLevel;
}

int Player::getWeaponDamageLevel() const
{
    return m_weaponDamageLevel;
}

int Player::getAttackSpeedLevel() const
{
    return m_attackSpeedLevel;
}

float Player::getWeaponDamageMultiplier() const
{
    return 1.f + static_cast<float>(m_weaponDamageLevel) * 0.18f;
}

float Player::getAttackCooldownMultiplier() const
{
    return std::max(0.45f, 1.f - static_cast<float>(m_attackSpeedLevel) * 0.08f);
}

bool Player::isDead() const
{
    return m_health <= 0;
}

int Player::getHealth() const
{
    return m_health;
}

int Player::getMaxHealth() const
{
    return m_maxHealth;
}

float Player::getSpeedBoostLeft() const
{
    return m_speedBoostTimer;
}

void Player::setWeapon(WeaponType type)
{
    if (type == WeaponType::Sword)
        m_weapon = std::make_unique<Sword>();
    else
        m_weapon = std::make_unique<Bow>();
}

void Player::tryMove(sf::Vector2f delta, const DungeonMap& map)
{
    const sf::Vector2f next = m_position + delta;
    if (!map.isBlocked({next, m_size}))
        m_position = next;
}
