#pragma once

#include <SFML/Graphics.hpp>

#include <filesystem>

enum class SpriteId
{
    KnightDown,
    KnightUp,
    KnightLeft,
    KnightRight,
    SkeletonDown,
    SkeletonUp,
    SkeletonLeft,
    SkeletonRight,
    ArmoredSkeletonDown,
    ArmoredSkeletonUp,
    ArmoredSkeletonLeft,
    ArmoredSkeletonRight,
    MageDown,
    MageUp,
    MageLeft,
    MageRight,
    FloorA,
    FloorB,
    WallA,
    WallB,
    WallCorner,
    HealthPack,
    SpeedPotion,
    Coin,
    SwordIcon,
    BowIcon,
    StaffIcon
};

class TextureAtlas
{
public:
    bool loadFromFile(const std::filesystem::path& path);
    bool isLoaded() const;

    void drawSprite(sf::RenderWindow& window, SpriteId id, sf::Vector2f center, sf::Vector2f targetSize) const;
    void drawTile(sf::RenderWindow& window, SpriteId id, sf::Vector2f topLeft, sf::Vector2f targetSize) const;

private:
    sf::Texture m_texture;
    bool m_loaded = false;

    sf::IntRect getRect(SpriteId id) const;
};
