#include "WaveManager.hpp"

#include "DungeonMap.hpp"
#include "Skeleton.hpp"

#include <algorithm>

void WaveManager::reset()
{
    m_wave = 0;
    m_pauseTimer = 1.2f;
    m_waiting = true;
}

void WaveManager::update(float dt)
{
    if (!m_waiting)
        return;

    m_pauseTimer -= dt;
}

void WaveManager::startNextWaveDelay()
{
    if (!m_waiting)
    {
        m_waiting = true;
        m_pauseTimer = 2.2f;
    }
}

bool WaveManager::isWaiting() const
{
    return m_waiting;
}

bool WaveManager::isReadyToSpawn() const
{
    return m_waiting && m_pauseTimer <= 0.f;
}

void WaveManager::spawnWave(std::vector<std::unique_ptr<Enemy>>& enemies, const DungeonMap& map, sf::Vector2f playerPosition, std::mt19937& rng, float survivalTime)
{
    ++m_wave;
    const int timeBonus = std::min(10, static_cast<int>(survivalTime / 30.f));
    const int difficulty = m_wave + timeBonus;
    const int count = 4 + m_wave * 2 + timeBonus;
    const bool armored = survivalTime >= 300.f;
    const int mageCount = survivalTime >= 180.f ? std::min(2, 1 + m_wave / 4) : 0;

    enemies.clear();
    enemies.reserve(static_cast<std::size_t>(count));

    for (int i = 0; i < count; ++i)
    {
        const sf::Vector2f position = map.randomFreePosition(rng, playerPosition);
        if (i < mageCount)
            enemies.push_back(std::make_unique<MageSkeleton>(position, difficulty, armored));
        else
            enemies.push_back(std::make_unique<Skeleton>(position, difficulty, armored));
    }

    m_waiting = false;
    m_pauseTimer = 0.f;
}

int WaveManager::getWave() const
{
    return m_wave;
}

float WaveManager::getPauseLeft() const
{
    return m_pauseTimer;
}
