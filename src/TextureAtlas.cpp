#include "TextureAtlas.hpp"

bool TextureAtlas::loadFromFile(const std::filesystem::path& path)
{
    sf::Image image;
    if (!image.loadFromFile(path))
    {
        m_loaded = false;
        return false;
    }

    const sf::Vector2u size = image.getSize();
    for (unsigned int y = 0; y < size.y; ++y)
    {
        for (unsigned int x = 0; x < size.x; ++x)
        {
            sf::Color color = image.getPixel({x, y});
            if (color.r < 24 && color.g < 28 && color.b < 32)
            {
                color.a = 0;
                image.setPixel({x, y}, color);
            }
        }
    }

    m_loaded = m_texture.loadFromImage(image);
    if (m_loaded)
        m_texture.setSmooth(false);

    return m_loaded;
}

bool TextureAtlas::isLoaded() const
{
    return m_loaded;
}

void TextureAtlas::drawSprite(sf::RenderWindow& window, SpriteId id, sf::Vector2f center, sf::Vector2f targetSize) const
{
    if (!m_loaded)
        return;

    const sf::IntRect rect = getRect(id);
    sf::Sprite sprite(m_texture, rect);
    const auto rectSize = rect.size;
    sprite.setOrigin({static_cast<float>(rectSize.x) * 0.5f, static_cast<float>(rectSize.y) * 0.5f});
    sprite.setPosition(center);
    sprite.setScale({
        targetSize.x / static_cast<float>(rectSize.x),
        targetSize.y / static_cast<float>(rectSize.y)
    });
    window.draw(sprite);
}

void TextureAtlas::drawTile(sf::RenderWindow& window, SpriteId id, sf::Vector2f topLeft, sf::Vector2f targetSize) const
{
    if (!m_loaded)
        return;

    const sf::IntRect rect = getRect(id);
    sf::Sprite sprite(m_texture, rect);
    const auto rectSize = rect.size;
    sprite.setPosition(topLeft);
    sprite.setScale({
        targetSize.x / static_cast<float>(rectSize.x),
        targetSize.y / static_cast<float>(rectSize.y)
    });
    window.draw(sprite);
}

sf::IntRect TextureAtlas::getRect(SpriteId id) const
{
    switch (id)
    {
        case SpriteId::KnightDown: return {{92, 112}, {64, 82}};
        case SpriteId::KnightUp: return {{92, 232}, {64, 82}};
        case SpriteId::KnightLeft: return {{92, 354}, {64, 82}};
        case SpriteId::KnightRight: return {{92, 472}, {64, 82}};

        case SpriteId::MageDown: return {{412, 112}, {70, 86}};
        case SpriteId::MageUp: return {{412, 232}, {70, 86}};
        case SpriteId::MageLeft: return {{412, 354}, {70, 86}};
        case SpriteId::MageRight: return {{412, 472}, {70, 86}};

        case SpriteId::SkeletonDown: return {{744, 112}, {66, 82}};
        case SpriteId::SkeletonUp: return {{744, 232}, {66, 82}};
        case SpriteId::SkeletonLeft: return {{744, 354}, {66, 82}};
        case SpriteId::SkeletonRight: return {{744, 472}, {66, 82}};

        case SpriteId::ArmoredSkeletonDown: return {{1090, 112}, {72, 86}};
        case SpriteId::ArmoredSkeletonUp: return {{1090, 232}, {72, 86}};
        case SpriteId::ArmoredSkeletonLeft: return {{1090, 354}, {72, 86}};
        case SpriteId::ArmoredSkeletonRight: return {{1090, 472}, {72, 86}};

        case SpriteId::FloorA: return {{20, 680}, {58, 58}};
        case SpriteId::FloorB: return {{151, 680}, {58, 58}};
        case SpriteId::WallA: return {{383, 680}, {51, 59}};
        case SpriteId::WallB: return {{508, 680}, {51, 59}};
        case SpriteId::WallCorner: return {{383, 752}, {51, 59}};

        case SpriteId::HealthPack: return {{1160, 696}, {48, 48}};
        case SpriteId::SpeedPotion: return {{1254, 695}, {45, 48}};
        case SpriteId::Coin: return {{1336, 695}, {43, 45}};
        case SpriteId::SwordIcon: return {{831, 832}, {56, 56}};
        case SpriteId::BowIcon: return {{904, 832}, {56, 56}};
        case SpriteId::StaffIcon: return {{974, 832}, {56, 56}};
    }

    return {{20, 680}, {58, 58}};
}
