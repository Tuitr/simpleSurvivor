#include "Weapon.hpp"

WeaponType Sword::type() const
{
    return WeaponType::Sword;
}

std::string Sword::name() const
{
    return "Sword";
}

float Sword::cooldown() const
{
    return 0.34f;
}

int Sword::damage() const
{
    return 36;
}

sf::FloatRect Sword::hitBox(sf::Vector2f playerCenter, sf::Vector2f facing) const
{
    facing = normalize(facing);
    const bool horizontal = std::abs(facing.x) > std::abs(facing.y);
    const sf::Vector2f size = horizontal ? sf::Vector2f{62.f, 42.f} : sf::Vector2f{42.f, 62.f};
    const sf::Vector2f center = playerCenter + facing * 42.f;
    return makeRect(center, size);
}

void Sword::drawHitBox(sf::RenderWindow& window, const sf::FloatRect& hitBox) const
{
    sf::RectangleShape slash(hitBox.size);
    slash.setPosition(hitBox.position);
    slash.setFillColor(sf::Color(246, 221, 124, 105));
    slash.setOutlineColor(sf::Color(255, 245, 180, 180));
    slash.setOutlineThickness(2.f);
    window.draw(slash);
}

void Sword::drawSwing(sf::RenderWindow& window, sf::Vector2f playerCenter, sf::Vector2f facing) const
{
    const sf::Vector2f direction = normalize(facing);
    const float angle = std::atan2(direction.y, direction.x);
    const sf::Vector2f base = playerCenter + direction * 18.f;
    const sf::Vector2f tip = playerCenter + direction * 58.f;

    sf::RectangleShape handle({18.f, 7.f});
    handle.setOrigin({3.f, 3.5f});
    handle.setPosition(base);
    handle.setRotation(sf::radians(angle));
    handle.setFillColor(sf::Color(92, 58, 36));
    window.draw(handle);

    sf::RectangleShape guard({8.f, 24.f});
    guard.setOrigin({4.f, 12.f});
    guard.setPosition(playerCenter + direction * 25.f);
    guard.setRotation(sf::radians(angle));
    guard.setFillColor(sf::Color(214, 180, 82));
    window.draw(guard);

    sf::RectangleShape blade({42.f, 10.f});
    blade.setOrigin({0.f, 5.f});
    blade.setPosition(playerCenter + direction * 30.f);
    blade.setRotation(sf::radians(angle));
    blade.setFillColor(sf::Color(218, 228, 230));
    window.draw(blade);

    sf::RectangleShape highlight({30.f, 3.f});
    highlight.setOrigin({0.f, 1.5f});
    highlight.setPosition(playerCenter + direction * 36.f + sf::Vector2f{-direction.y, direction.x} * 2.f);
    highlight.setRotation(sf::radians(angle));
    highlight.setFillColor(sf::Color(255, 255, 245));
    window.draw(highlight);

    sf::RectangleShape point({8.f, 8.f});
    point.setOrigin({4.f, 4.f});
    point.setPosition(tip);
    point.setRotation(sf::radians(angle));
    point.setFillColor(sf::Color(232, 240, 240));
    window.draw(point);
}

WeaponType Bow::type() const
{
    return WeaponType::Bow;
}

std::string Bow::name() const
{
    return "Bow";
}

float Bow::cooldown() const
{
    return 0.55f;
}

Projectile Bow::shoot(sf::Vector2f playerCenter, sf::Vector2f facing) const
{
    return shoot(playerCenter, facing, damage());
}

Projectile Bow::shoot(sf::Vector2f playerCenter, sf::Vector2f facing, int damage) const
{
    const sf::Vector2f direction = normalize(facing);
    return Projectile(playerCenter + direction * 26.f, direction, damage);
}

int Bow::damage() const
{
    return 18;
}
