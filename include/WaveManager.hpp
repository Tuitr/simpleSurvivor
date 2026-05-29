#pragma once

#include "Enemy.hpp"

#include <memory>
#include <random>
#include <vector>

class DungeonMap;

class WaveManager
{
public:
    void reset();
    void update(float dt);
    void startNextWaveDelay();
    bool isWaiting() const;
    bool isReadyToSpawn() const;
    void spawnWave(std::vector<std::unique_ptr<Enemy>>& enemies, const DungeonMap& map, sf::Vector2f playerPosition, std::mt19937& rng, float survivalTime);
    int getWave() const;
    float getPauseLeft() const;

private:
    int m_wave = 0;
    float m_pauseTimer = 1.2f;
    bool m_waiting = true;
};
