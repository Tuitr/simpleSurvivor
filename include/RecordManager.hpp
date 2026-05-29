#pragma once

#include <filesystem>

class RecordManager
{
public:
    explicit RecordManager(std::filesystem::path path);

    float loadBestTime();
    void saveBestTime(float seconds);
    float getBestTime() const;
    bool updateIfBest(float seconds);

private:
    std::filesystem::path m_path;
    float m_bestTime = 0.f;
};
