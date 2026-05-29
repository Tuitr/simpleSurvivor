#include "DungeonMap.hpp"

#include "Common.hpp"
#include "TextureAtlas.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

DungeonMap::DungeonMap()
    : m_size(2600.f, 1800.f)
{
    m_walls.push_back({{360.f, 260.f}, {260.f, 42.f}});
    m_walls.push_back({{880.f, 420.f}, {50.f, 340.f}});
    m_walls.push_back({{1390.f, 250.f}, {360.f, 46.f}});
    m_walls.push_back({{1760.f, 690.f}, {62.f, 360.f}});
    m_walls.push_back({{530.f, 1160.f}, {420.f, 54.f}});
    m_walls.push_back({{1260.f, 1240.f}, {54.f, 300.f}});
    m_walls.push_back({{1700.f, 1320.f}, {380.f, 54.f}});

    m_decor.push_back({{230.f, 500.f}, {34.f, 34.f}});
    m_decor.push_back({{730.f, 790.f}, {42.f, 28.f}});
    m_decor.push_back({{1180.f, 850.f}, {30.f, 30.f}});
    m_decor.push_back({{2100.f, 430.f}, {46.f, 32.f}});
    m_decor.push_back({{2150.f, 1220.f}, {36.f, 42.f}});
    m_decor.push_back({{430.f, 430.f}, {18.f, 18.f}});
    m_decor.push_back({{480.f, 470.f}, {12.f, 12.f}});
    m_decor.push_back({{1040.f, 210.f}, {24.f, 36.f}});
    m_decor.push_back({{1090.f, 210.f}, {24.f, 36.f}});
    m_decor.push_back({{1480.f, 820.f}, {54.f, 18.f}});
    m_decor.push_back({{1560.f, 855.f}, {20.f, 20.f}});
    m_decor.push_back({{1940.f, 260.f}, {16.f, 16.f}});
    m_decor.push_back({{1990.f, 295.f}, {12.f, 12.f}});
    m_decor.push_back({{320.f, 1440.f}, {42.f, 20.f}});
    m_decor.push_back({{380.f, 1480.f}, {16.f, 16.f}});
    m_decor.push_back({{1090.f, 1450.f}, {34.f, 18.f}});
    m_decor.push_back({{2240.f, 1510.f}, {58.f, 22.f}});
    m_decor.push_back({{2380.f, 980.f}, {18.f, 46.f}});
    m_decor.push_back({{250.f, 950.f}, {22.f, 50.f}});
}

void DungeonMap::draw(sf::RenderWindow& window, const TextureAtlas* atlas) const
{
    sf::RectangleShape floor({32.f, 32.f});

    for (int y = 0; y < static_cast<int>(m_size.y); y += 32)
    {
        for (int x = 0; x < static_cast<int>(m_size.x); x += 32)
        {
            const bool alt = ((x / 32 + y / 32) % 2) == 0;
            if (atlas && atlas->isLoaded())
            {
                atlas->drawTile(window, alt ? SpriteId::FloorA : SpriteId::FloorB, {static_cast<float>(x), static_cast<float>(y)}, {32.f, 32.f});
                continue;
            }

            floor.setPosition({static_cast<float>(x), static_cast<float>(y)});
            floor.setFillColor(alt ? sf::Color(38, 38, 48) : sf::Color(33, 33, 42));
            window.draw(floor);
        }
    }

    sf::VertexArray grid(sf::PrimitiveType::Lines);
    for (float x = 0.f; x <= m_size.x; x += 128.f)
    {
        grid.append({{x, 0.f}, sf::Color(49, 49, 62)});
        grid.append({{x, m_size.y}, sf::Color(49, 49, 62)});
    }
    for (float y = 0.f; y <= m_size.y; y += 128.f)
    {
        grid.append({{0.f, y}, sf::Color(49, 49, 62)});
        grid.append({{m_size.x, y}, sf::Color(49, 49, 62)});
    }
    window.draw(grid);

    drawRect(window, {{0.f, 0.f}, {m_size.x, 28.f}}, sf::Color(82, 82, 92));
    drawRect(window, {{0.f, m_size.y - 28.f}, {m_size.x, 28.f}}, sf::Color(82, 82, 92));
    drawRect(window, {{0.f, 0.f}, {28.f, m_size.y}}, sf::Color(82, 82, 92));
    drawRect(window, {{m_size.x - 28.f, 0.f}, {28.f, m_size.y}}, sf::Color(82, 82, 92));

    for (std::size_t i = 0; i < m_walls.size(); ++i)
    {
        const auto& wall = m_walls[i];
        if (atlas && atlas->isLoaded())
        {
            for (float y = wall.position.y; y < wall.position.y + wall.size.y; y += 32.f)
            {
                for (float x = wall.position.x; x < wall.position.x + wall.size.x; x += 32.f)
                {
                    const SpriteId id = (i + static_cast<std::size_t>(x + y)) % 3 == 0 ? SpriteId::WallCorner : SpriteId::WallA;
                    atlas->drawTile(window, id, {x, y}, {32.f, 32.f});
                }
            }
            continue;
        }
        drawRect(window, wall, sf::Color(94, 91, 108));
    }

    for (std::size_t i = 0; i < m_decor.size(); ++i)
    {
        const auto& decor = m_decor[i];
        const sf::Color color = (i % 3 == 0) ? sf::Color(74, 72, 84) : sf::Color(58, 58, 70);
        drawRect(window, decor, color);

        if (decor.size.y > 35.f)
        {
            sf::RectangleShape flame({10.f, 10.f});
            flame.setPosition({decor.position.x + decor.size.x * 0.5f - 5.f, decor.position.y - 10.f});
            flame.setFillColor(sf::Color(224, 111, 54));
            window.draw(flame);

            sf::RectangleShape glow({6.f, 6.f});
            glow.setPosition({decor.position.x + decor.size.x * 0.5f - 3.f, decor.position.y - 8.f});
            glow.setFillColor(sf::Color(246, 205, 94));
            window.draw(glow);
        }
    }
}

bool DungeonMap::isBlocked(const sf::FloatRect& bounds) const
{
    if (bounds.position.x < 28.f || bounds.position.y < 28.f ||
        bounds.position.x + bounds.size.x > m_size.x - 28.f ||
        bounds.position.y + bounds.size.y > m_size.y - 28.f)
    {
        return true;
    }

    for (const auto& wall : m_walls)
    {
        if (intersects(bounds, wall))
            return true;
    }

    return false;
}

sf::Vector2f DungeonMap::randomFreePosition(std::mt19937& rng, sf::Vector2f awayFrom) const
{
    std::uniform_real_distribution<float> xDist(80.f, m_size.x - 80.f);
    std::uniform_real_distribution<float> yDist(80.f, m_size.y - 80.f);

    for (int attempt = 0; attempt < 120; ++attempt)
    {
        sf::Vector2f position{xDist(rng), yDist(rng)};
        if (vectorLength(position - awayFrom) < 320.f)
            continue;

        if (!isBlocked(makeRect(position, {34.f, 34.f})))
            return position;
    }

    return {m_size.x * 0.5f, m_size.y * 0.5f};
}

sf::Vector2f DungeonMap::getSize() const
{
    return m_size;
}

void DungeonMap::drawRect(sf::RenderWindow& window, const sf::FloatRect& rect, sf::Color color) const
{
    sf::RectangleShape shape(rect.size);
    shape.setPosition(rect.position);
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color(28, 28, 34));
    shape.setOutlineThickness(2.f);
    window.draw(shape);

    if (rect.size.x < 45.f && rect.size.y < 45.f)
        return;

    sf::RectangleShape chip;
    chip.setFillColor(sf::Color(
        static_cast<std::uint8_t>(std::max(0, color.r - 24)),
        static_cast<std::uint8_t>(std::max(0, color.g - 24)),
        static_cast<std::uint8_t>(std::max(0, color.b - 24))));

    const int pieces = static_cast<int>((rect.size.x + rect.size.y) / 90.f);
    for (int i = 0; i < pieces; ++i)
    {
        const float seed = rect.position.x * 0.37f + rect.position.y * 0.19f + static_cast<float>(i * 41);
        const bool horizontal = (i % 2) == 0;
        const float along = horizontal
            ? std::fmod(seed * 13.f, std::max(16.f, rect.size.x - 18.f))
            : std::fmod(seed * 17.f, std::max(16.f, rect.size.y - 18.f));
        const float edge = std::fmod(seed, 2.f) < 1.f ? 0.f : 1.f;

        if (horizontal)
        {
            chip.setSize({18.f + static_cast<float>((i % 3) * 5), 7.f});
            chip.setPosition({rect.position.x + along, rect.position.y + (edge > 0.f ? rect.size.y - 7.f : 0.f)});
        }
        else
        {
            chip.setSize({7.f, 18.f + static_cast<float>((i % 3) * 5)});
            chip.setPosition({rect.position.x + (edge > 0.f ? rect.size.x - 7.f : 0.f), rect.position.y + along});
        }

        window.draw(chip);
    }
}
