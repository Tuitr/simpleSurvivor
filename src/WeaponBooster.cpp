#include "WeaponBooster.hpp"

#include "TextureAtlas.hpp"

WeaponBooster::WeaponBooster(sf::Vector2f position, WeaponUpgradeType type)
    : m_position(position),
      m_size(28.f, 28.f),
      m_type(type)
{
}

void WeaponBooster::draw(sf::RenderWindow& window, const TextureAtlas* atlas) const
{
    const bool damage = m_type == WeaponUpgradeType::Damage;
    if (atlas && atlas->isLoaded())
    {
        atlas->drawSprite(window, damage ? SpriteId::SwordIcon : SpriteId::BowIcon, {m_position.x + m_size.x * 0.5f, m_position.y + m_size.y * 0.5f}, {38.f, 38.f});
        return;
    }

    sf::RectangleShape box(m_size);
    box.setPosition(m_position);
    box.setFillColor(damage ? sf::Color(86, 42, 46) : sf::Color(44, 61, 96));
    box.setOutlineColor(damage ? sf::Color(255, 181, 96) : sf::Color(138, 202, 255));
    box.setOutlineThickness(2.f);
    window.draw(box);

    if (damage)
    {
        sf::RectangleShape blade({5.f, 18.f});
        blade.setPosition({m_position.x + 12.f, m_position.y + 4.f});
        blade.setFillColor(sf::Color(238, 231, 196));
        window.draw(blade);

        sf::RectangleShape guard({15.f, 4.f});
        guard.setPosition({m_position.x + 7.f, m_position.y + 16.f});
        guard.setFillColor(sf::Color(246, 190, 88));
        window.draw(guard);
    }
    else
    {
        sf::RectangleShape spark({18.f, 5.f});
        spark.setPosition({m_position.x + 5.f, m_position.y + 8.f});
        spark.setFillColor(sf::Color(151, 223, 255));
        window.draw(spark);

        sf::RectangleShape spark2({12.f, 5.f});
        spark2.setPosition({m_position.x + 9.f, m_position.y + 17.f});
        spark2.setFillColor(sf::Color(151, 223, 255));
        window.draw(spark2);
    }
}

sf::FloatRect WeaponBooster::getBounds() const
{
    return {m_position, m_size};
}

WeaponUpgradeType WeaponBooster::getType() const
{
    return m_type;
}
