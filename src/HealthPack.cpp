#include "HealthPack.hpp"

#include "TextureAtlas.hpp"

HealthPack::HealthPack(sf::Vector2f position)
    : m_position(position),
      m_size(26.f, 26.f)
{
}

void HealthPack::draw(sf::RenderWindow& window, const TextureAtlas* atlas) const
{
    if (atlas && atlas->isLoaded())
    {
        atlas->drawSprite(window, SpriteId::HealthPack, {m_position.x + m_size.x * 0.5f, m_position.y + m_size.y * 0.5f}, {34.f, 34.f});
        return;
    }

    sf::RectangleShape box(m_size);
    box.setPosition(m_position);
    box.setFillColor(sf::Color(235, 235, 225));
    box.setOutlineColor(sf::Color(110, 42, 48));
    box.setOutlineThickness(2.f);
    window.draw(box);

    sf::RectangleShape vertical({6.f, 18.f});
    vertical.setPosition({m_position.x + 10.f, m_position.y + 4.f});
    vertical.setFillColor(sf::Color(190, 38, 52));
    window.draw(vertical);

    sf::RectangleShape horizontal({18.f, 6.f});
    horizontal.setPosition({m_position.x + 4.f, m_position.y + 10.f});
    horizontal.setFillColor(sf::Color(190, 38, 52));
    window.draw(horizontal);
}

sf::FloatRect HealthPack::getBounds() const
{
    return {m_position, m_size};
}

int HealthPack::getHealAmount() const
{
    return m_healAmount;
}
