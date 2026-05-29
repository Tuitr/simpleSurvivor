#include "Button.hpp"

Button::Button(sf::Vector2f position, sf::Vector2f size, const std::string& label)
    : m_label(label)
{
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color(54, 58, 75));
    m_shape.setOutlineColor(sf::Color(190, 176, 114));
    m_shape.setOutlineThickness(3.f);
}

void Button::setPosition(sf::Vector2f position)
{
    m_shape.setPosition(position);
}

bool Button::contains(sf::Vector2f point) const
{
    return m_shape.getGlobalBounds().contains(point);
}

void Button::draw(sf::RenderWindow& window, const sf::Font* font) const
{
    window.draw(m_shape);

    if (!font)
        return;

    sf::Text text(*font, m_label, 24);
    text.setFillColor(sf::Color(236, 232, 204));
    const sf::FloatRect bounds = text.getLocalBounds();
    const sf::FloatRect button = m_shape.getGlobalBounds();
    text.setPosition({
        button.position.x + (button.size.x - bounds.size.x) * 0.5f - bounds.position.x,
        button.position.y + (button.size.y - bounds.size.y) * 0.5f - bounds.position.y - 2.f
    });
    window.draw(text);
}
