#include "Skeleton.hpp"

#include "DungeonMap.hpp"
#include "TextureAtlas.hpp"

#include <cmath>

Skeleton::Skeleton(sf::Vector2f position, int wave, bool armored)
    : m_position(position),
      m_size(30.f, 30.f)
{
    m_maxHealth = 28 + wave * 7;
    m_health = m_maxHealth;
    m_speed = 74.f + wave * 5.f;
    m_damage = 7 + wave;
    m_strafeSign = (static_cast<int>(position.x + position.y) % 2 == 0) ? 1 : -1;

    if (armored)
        setArmored(true);
}

void Skeleton::update(float dt, const sf::Vector2f& playerPosition, const DungeonMap& map)
{
    m_hitCooldown -= dt;

    const sf::Vector2f toPlayer = playerPosition - getCenter();
    const float distanceToPlayer = vectorLength(toPlayer);
    const float stepDistance = m_speed * dt;
    constexpr float stopDistance = 44.f;
    sf::Vector2f direction = normalize(toPlayer);
    const sf::Vector2f side{-direction.y * static_cast<float>(m_strafeSign), direction.x * static_cast<float>(m_strafeSign)};

    if (distanceToPlayer <= stopDistance + stepDistance)
    {
        direction = side;
    }
    else if (distanceToPlayer < 220.f)
    {
        direction = normalize(direction * 0.45f + side * 0.55f);
    }

    trySmartMove(direction, stepDistance, map);
}

void Skeleton::trySmartMove(sf::Vector2f direction, float distance, const DungeonMap& map)
{
    const sf::Vector2f direct = direction * distance;
    if (tryMove(direct, map))
        return;

    if (tryMove({direct.x, 0.f}, map))
        return;

    if (tryMove({0.f, direct.y}, map))
        return;

    const sf::Vector2f side{-direction.y * static_cast<float>(m_strafeSign), direction.x * static_cast<float>(m_strafeSign)};
    if (tryMove(side * distance * 0.85f, map))
        return;

    m_strafeSign *= -1;
    tryMove(side * -distance * 0.85f, map);
}

bool Skeleton::tryMove(sf::Vector2f delta, const DungeonMap& map)
{
    sf::Vector2f next = m_position;
    next += delta;
    if (map.isBlocked({next, m_size}))
        return false;

    m_position = next;
    return true;
}

void Skeleton::draw(sf::RenderWindow& window, const TextureAtlas* atlas) const
{
    if (atlas && atlas->isLoaded())
    {
        SpriteId id = m_armored ? SpriteId::ArmoredSkeletonDown : SpriteId::SkeletonDown;
        if (m_armored)
            id = SpriteId::ArmoredSkeletonDown;

        atlas->drawSprite(window, id, getCenter() + sf::Vector2f{0.f, -8.f}, m_armored ? sf::Vector2f{58.f, 72.f} : sf::Vector2f{52.f, 66.f});

        sf::RectangleShape healthBack({32.f, 5.f});
        healthBack.setPosition({m_position.x - 1.f, m_position.y - 12.f});
        healthBack.setFillColor(sf::Color(44, 22, 28));
        window.draw(healthBack);

        sf::RectangleShape healthFill({32.f * std::max(0.f, getHealthRatio()), 5.f});
        healthFill.setPosition({m_position.x - 1.f, m_position.y - 12.f});
        healthFill.setFillColor(m_armored ? sf::Color(117, 174, 205) : sf::Color(190, 54, 64));
        window.draw(healthFill);
        return;
    }

    sf::RectangleShape body({22.f, 22.f});
    body.setPosition({m_position.x + 4.f, m_position.y + 7.f});
    body.setFillColor(sf::Color(218, 213, 185));
    window.draw(body);

    if (m_armored)
    {
        sf::RectangleShape chest({24.f, 16.f});
        chest.setPosition({m_position.x + 3.f, m_position.y + 9.f});
        chest.setFillColor(sf::Color(94, 112, 132));
        window.draw(chest);

        sf::RectangleShape shine({5.f, 14.f});
        shine.setPosition({m_position.x + 8.f, m_position.y + 10.f});
        shine.setFillColor(sf::Color(151, 170, 188));
        window.draw(shine);
    }

    sf::RectangleShape head({18.f, 14.f});
    head.setPosition({m_position.x + 6.f, m_position.y});
    head.setFillColor(sf::Color(235, 231, 204));
    window.draw(head);

    if (m_armored)
    {
        sf::RectangleShape helmet({22.f, 7.f});
        helmet.setPosition({m_position.x + 4.f, m_position.y - 2.f});
        helmet.setFillColor(sf::Color(105, 121, 141));
        window.draw(helmet);

        sf::RectangleShape horns({5.f, 5.f});
        horns.setFillColor(sf::Color(198, 195, 178));
        horns.setPosition({m_position.x + 1.f, m_position.y - 1.f});
        window.draw(horns);
        horns.setPosition({m_position.x + 24.f, m_position.y - 1.f});
        window.draw(horns);
    }

    sf::RectangleShape eye({4.f, 4.f});
    eye.setFillColor(sf::Color(35, 35, 42));
    eye.setPosition({m_position.x + 10.f, m_position.y + 5.f});
    window.draw(eye);
    eye.setPosition({m_position.x + 18.f, m_position.y + 5.f});
    window.draw(eye);

    sf::RectangleShape bone({6.f, 16.f});
    bone.setFillColor(sf::Color(197, 192, 168));
    bone.setPosition({m_position.x, m_position.y + 10.f});
    window.draw(bone);
    bone.setPosition({m_position.x + 24.f, m_position.y + 10.f});
    window.draw(bone);

    sf::RectangleShape healthBack({32.f, 5.f});
    healthBack.setPosition({m_position.x - 1.f, m_position.y - 12.f});
    healthBack.setFillColor(sf::Color(44, 22, 28));
    window.draw(healthBack);

    sf::RectangleShape healthFill({32.f * std::max(0.f, getHealthRatio()), 5.f});
    healthFill.setPosition({m_position.x - 1.f, m_position.y - 12.f});
    healthFill.setFillColor(m_armored ? sf::Color(117, 174, 205) : sf::Color(190, 54, 64));
    window.draw(healthFill);
}

sf::FloatRect Skeleton::getBounds() const
{
    return {m_position, m_size};
}

sf::Vector2f Skeleton::getCenter() const
{
    return {m_position.x + m_size.x * 0.5f, m_position.y + m_size.y * 0.5f};
}

int Skeleton::getDamage() const
{
    return m_damage;
}

bool Skeleton::canHitPlayer() const
{
    return m_hitCooldown <= 0.f;
}

void Skeleton::restartHitCooldown()
{
    m_hitCooldown = 0.75f;
}

void Skeleton::setArmored(bool armored)
{
    if (!armored || m_armored)
        return;

    m_armored = true;
    m_maxHealth += 45;
    m_health += 45;
    m_damage += 4;
    m_speed += 8.f;
}

bool Skeleton::isArmored() const
{
    return m_armored;
}

MageSkeleton::MageSkeleton(sf::Vector2f position, int wave, bool armored)
    : Skeleton(position, wave + 1, armored)
{
    m_maxHealth += 18;
    m_health += 18;
    m_damage += 3;
    m_speed -= 8.f;
}

void MageSkeleton::update(float dt, const sf::Vector2f& playerPosition, const DungeonMap& map)
{
    m_hitCooldown -= dt;
    m_shootCooldown -= dt;

    const sf::Vector2f toPlayer = playerPosition - getCenter();
    const float distance = vectorLength(toPlayer);
    sf::Vector2f direction = normalize(toPlayer);
    const sf::Vector2f side{-direction.y * static_cast<float>(m_strafeSign), direction.x * static_cast<float>(m_strafeSign)};

    if (distance < 250.f)
        direction = normalize(direction * -1.f + side * 0.35f);
    else if (distance > 520.f)
        direction = normalize(direction + side * 0.15f);
    else
        direction = side;

    trySmartMove(direction, m_speed * 0.72f * dt, map);
}

void MageSkeleton::draw(sf::RenderWindow& window, const TextureAtlas* atlas) const
{
    if (atlas && atlas->isLoaded())
    {
        atlas->drawSprite(window, SpriteId::MageDown, getCenter() + sf::Vector2f{0.f, -10.f}, {58.f, 72.f});

        sf::RectangleShape healthBack({32.f, 5.f});
        healthBack.setPosition({m_position.x - 1.f, m_position.y - 12.f});
        healthBack.setFillColor(sf::Color(44, 22, 28));
        window.draw(healthBack);

        sf::RectangleShape healthFill({32.f * std::max(0.f, getHealthRatio()), 5.f});
        healthFill.setPosition({m_position.x - 1.f, m_position.y - 12.f});
        healthFill.setFillColor(sf::Color(205, 128, 244));
        window.draw(healthFill);
        return;
    }

    Skeleton::draw(window, atlas);

    sf::RectangleShape robe({24.f, 20.f});
    robe.setPosition({m_position.x + 3.f, m_position.y + 8.f});
    robe.setFillColor(sf::Color(80, 48, 122, 210));
    window.draw(robe);

    sf::RectangleShape hood({20.f, 12.f});
    hood.setPosition({m_position.x + 5.f, m_position.y - 1.f});
    hood.setFillColor(sf::Color(104, 59, 154));
    window.draw(hood);

    sf::RectangleShape staff({5.f, 30.f});
    staff.setPosition({m_position.x + 30.f, m_position.y - 4.f});
    staff.setFillColor(sf::Color(86, 55, 36));
    window.draw(staff);

    sf::RectangleShape gem({9.f, 9.f});
    gem.setPosition({m_position.x + 28.f, m_position.y - 10.f});
    gem.setFillColor(sf::Color(204, 109, 244));
    window.draw(gem);
}

bool MageSkeleton::isMage() const
{
    return true;
}

bool MageSkeleton::canShoot() const
{
    return m_shootCooldown <= 0.f;
}

Projectile MageSkeleton::shootAt(sf::Vector2f target)
{
    m_shootCooldown = 1.65f;
    const sf::Vector2f direction = normalize(target - getCenter());
    return Projectile(getCenter() + direction * 18.f, direction, m_damage + 4, true, 330.f, 780.f);
}
