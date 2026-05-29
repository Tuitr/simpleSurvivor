#include "RecordManager.hpp"

#include <fstream>

RecordManager::RecordManager(std::filesystem::path path)
    : m_path(std::move(path))
{
}

float RecordManager::loadBestTime()
{
    std::ifstream input(m_path);
    if (!(input >> m_bestTime))
        m_bestTime = 0.f;

    return m_bestTime;
}

void RecordManager::saveBestTime(float seconds)
{
    std::filesystem::create_directories(m_path.parent_path());
    std::ofstream output(m_path, std::ios::trunc);
    output << seconds;
}

float RecordManager::getBestTime() const
{
    return m_bestTime;
}

bool RecordManager::updateIfBest(float seconds)
{
    if (seconds <= m_bestTime)
        return false;

    m_bestTime = seconds;
    saveBestTime(m_bestTime);
    return true;
}
