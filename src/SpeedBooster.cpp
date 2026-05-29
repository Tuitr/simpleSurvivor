#include "SpeedBooster.hpp"

#include "TextureAtlas.hpp"

SpeedBooster::SpeedBooster(sf::Vector2f position)
    : m_position(position),
      m_size(26.f, 26.f)
{
}

void SpeedBooster::draw(sf::RenderWindow& window, const TextureAtlas* atlas) const
{
    if (atlas && atlas->isLoaded())
    {
        atlas->drawSprite(window, SpriteId::SpeedPotion, {m_position.x + m_size.x * 0.5f, m_position.y + m_size.y * 0.5f}, {34.f, 34.f});
        return;
    }

    sf::RectangleShape base(m_size);
    base.setPosition(m_position);
    base.setFillColor(sf::Color(35, 72, 79));
    base.setOutlineColor(sf::Color(76, 224, 188));
    base.setOutlineThickness(2.f);
    window.draw(base);

    sf::RectangleShape bolt({6.f, 18.f});
    bolt.setFillColor(sf::Color(117, 255, 211));
    bolt.setPosition({m_position.x + 11.f, m_position.y + 4.f});
    window.draw(bolt);

    sf::RectangleShape wing({10.f, 5.f});
    wing.setFillColor(sf::Color(92, 199, 229));
    wing.setPosition({m_position.x + 4.f, m_position.y + 7.f});
    window.draw(wing);
    wing.setPosition({m_position.x + 12.f, m_position.y + 15.f});
    window.draw(wing);
}

sf::FloatRect SpeedBooster::getBounds() const
{
    return {m_position, m_size};
}

float SpeedBooster::getDuration() const
{
    return m_duration;
}

float SpeedBooster::getMultiplier() const
{
    return m_multiplier;
}
