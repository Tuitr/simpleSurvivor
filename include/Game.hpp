#pragma once

#include "Button.hpp"
#include "DungeonMap.hpp"
#include "Enemy.hpp"
#include "HealthPack.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "RecordManager.hpp"
#include "SpeedBooster.hpp"
#include "TextureAtlas.hpp"
#include "WaveManager.hpp"
#include "WeaponBooster.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <optional>
#include <random>
#include <vector>

class Game
{
public:
    Game();
    void run();

private:
    sf::RenderWindow m_window;
    sf::View m_gameView;
    sf::View m_uiView;
    sf::Clock m_clock;
    std::optional<sf::Font> m_font;
    TextureAtlas m_textureAtlas;

    GameState m_state = GameState::MainMenu;
    DungeonMap m_map;
    Player m_player;
    WaveManager m_waveManager;
    RecordManager m_recordManager;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<Projectile> m_projectiles;
    std::vector<HealthPack> m_healthPacks;
    std::vector<SpeedBooster> m_speedBoosters;
    std::vector<WeaponBooster> m_weaponBoosters;
    std::mt19937 m_rng;

    Button m_playButton;
    Button m_againButton;
    Button m_menuButton;
    Button m_exitButton;

    float m_survivalTime = 0.f;
    float m_finalTime = 0.f;
    float m_healthPackTimer = 0.f;
    float m_speedBoosterTimer = 0.f;
    float m_swordFlashTimer = 0.f;
    sf::FloatRect m_lastSwordBox;
    sf::Vector2f m_lastSwordFacing;
    bool m_newRecord = false;

    void handleEvents();
    void update(float dt);
    void draw();
    void drawMenu();
    void drawPlaying();
    void drawGameOver();
    void drawHud();
    void drawText(const std::string& text, sf::Vector2f position, unsigned int size, sf::Color color = sf::Color::White);
    void centerText(const std::string& text, sf::Vector2f center, unsigned int size, sf::Color color = sf::Color::White);

    void resetGame();
    void tryAttack();
    void checkCollisions();
    void spawnHealthPack();
    void spawnSpeedBooster();
    void spawnWeaponBooster();
    void removeDeadObjects();
    void finishGame();
    void updateViews();
    bool loadFont();
    bool loadTextureAtlas();
};
