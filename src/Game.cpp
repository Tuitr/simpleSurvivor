#include "Game.hpp"

#include "Skeleton.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <sstream>

std::string formatTime(float seconds)
{
    const int total = static_cast<int>(seconds);
    const int minutes = total / 60;
    const int secs = total % 60;

    std::ostringstream out;
    out << minutes << ':';
    if (secs < 10)
        out << '0';
    out << secs;
    return out.str();
}

Game::Game()
    : m_window(sf::VideoMode({1280u, 720u}), "Pixel Dungeon Survivor"),
      m_gameView({1280.f, 900.f}, {1280.f, 720.f}),
      m_uiView({640.f, 360.f}, {1280.f, 720.f}),
      m_recordManager(std::filesystem::path(PROJECT_ROOT) / "save" / "best_time.txt"),
      m_rng(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())),
      m_playButton({540.f, 390.f}, {200.f, 58.f}, "Play"),
      m_againButton({500.f, 390.f}, {280.f, 54.f}, "Play Again"),
      m_menuButton({500.f, 462.f}, {280.f, 54.f}, "Main Menu"),
      m_exitButton({500.f, 534.f}, {280.f, 54.f}, "Exit")
{
    m_window.setFramerateLimit(120);
    loadFont();
    loadTextureAtlas();
    m_recordManager.loadBestTime();
    m_player.reset({1280.f, 900.f});
}

void Game::run()
{
    while (m_window.isOpen())
    {
        const sf::Time elapsed = m_clock.restart();
        const float dt = std::min(elapsed.asSeconds(), 0.05f);
        handleEvents();
        update(dt);
        draw();
    }
}

void Game::handleEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            m_uiView.setSize({static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
            m_uiView.setCenter({static_cast<float>(resized->size.x) * 0.5f, static_cast<float>(resized->size.y) * 0.5f});
            m_gameView.setSize({static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
        }

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape && m_state == GameState::Playing)
                m_state = GameState::MainMenu;

            if (m_state == GameState::MainMenu && key->code == sf::Keyboard::Key::Enter)
                resetGame();

            if (m_state == GameState::Playing)
            {
                if (key->code == sf::Keyboard::Key::Q || key->code == sf::Keyboard::Key::Tab)
                    m_player.switchWeapon();
                if (key->code == sf::Keyboard::Key::Space)
                    tryAttack();
            }
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button != sf::Mouse::Button::Left)
                continue;

            const sf::Vector2f uiPoint = m_window.mapPixelToCoords(mouse->position, m_uiView);

            if (m_state == GameState::MainMenu && m_playButton.contains(uiPoint))
                resetGame();
            else if (m_state == GameState::Playing)
                tryAttack();
            else if (m_state == GameState::GameOver)
            {
                if (m_againButton.contains(uiPoint))
                    resetGame();
                else if (m_menuButton.contains(uiPoint))
                    m_state = GameState::MainMenu;
                else if (m_exitButton.contains(uiPoint))
                    m_window.close();
            }
        }
    }
}

void Game::update(float dt)
{
    if (m_state != GameState::Playing)
        return;

    m_survivalTime += dt;
    m_healthPackTimer += dt;
    m_speedBoosterTimer += dt;
    m_swordFlashTimer -= dt;

    m_gameView.setCenter(m_player.getCenter());
    const sf::Vector2f mouseWorld = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window), m_gameView);
    m_player.lookAt(mouseWorld);
    m_player.handleInput(dt, m_map);
    m_player.update(dt);

    m_waveManager.update(dt);
    if (m_waveManager.isReadyToSpawn())
    {
        m_waveManager.spawnWave(m_enemies, m_map, m_player.getCenter(), m_rng, m_survivalTime);
        spawnWeaponBooster();
    }

    if (m_survivalTime >= 300.f)
    {
        for (auto& enemy : m_enemies)
            enemy->setArmored(true);
    }

    for (auto& enemy : m_enemies)
    {
        enemy->update(dt, m_player.getCenter(), m_map);

        if (enemy->canShoot())
            m_projectiles.push_back(enemy->shootAt(m_player.getCenter()));
    }

    for (auto& projectile : m_projectiles)
        projectile.update(dt, m_map);

    if (m_healthPackTimer >= 8.f && m_healthPacks.size() < 4)
    {
        m_healthPackTimer = 0.f;
        spawnHealthPack();
    }

    if (m_speedBoosterTimer >= 13.f && m_speedBoosters.size() < 3)
    {
        m_speedBoosterTimer = 0.f;
        spawnSpeedBooster();
    }

    checkCollisions();
    removeDeadObjects();

    if (m_enemies.empty() && !m_waveManager.isWaiting())
        m_waveManager.startNextWaveDelay();

    if (m_player.isDead())
        finishGame();
}

void Game::draw()
{
    m_window.clear(sf::Color(16, 16, 22));

    if (m_state == GameState::MainMenu)
        drawMenu();
    else if (m_state == GameState::Playing)
        drawPlaying();
    else
        drawGameOver();

    m_window.display();
}

void Game::drawMenu()
{
    m_window.setView(m_uiView);
    centerText("PIXEL DUNGEON SURVIVOR", {640.f, 210.f}, 44, sf::Color(236, 221, 144));
    centerText("Best time: " + formatTime(m_recordManager.getBestTime()), {640.f, 292.f}, 24, sf::Color(220, 220, 210));
    m_playButton.draw(m_window, m_font ? &*m_font : nullptr);
    centerText("WASD move  |  Q/Tab weapon  |  Space/LMB attack", {640.f, 510.f}, 18, sf::Color(170, 170, 180));
}

void Game::drawPlaying()
{
    m_window.setView(m_gameView);
    const TextureAtlas* atlas = m_textureAtlas.isLoaded() ? &m_textureAtlas : nullptr;
    m_map.draw(m_window, atlas);

    for (const auto& pack : m_healthPacks)
        pack.draw(m_window);

    for (const auto& booster : m_speedBoosters)
        booster.draw(m_window);

    for (const auto& booster : m_weaponBoosters)
        booster.draw(m_window);

    for (const auto& projectile : m_projectiles)
        projectile.draw(m_window);

    for (const auto& enemy : m_enemies)
        enemy->draw(m_window);

    if (m_swordFlashTimer > 0.f)
    {
        Sword sword;
        sword.drawHitBox(m_window, m_lastSwordBox);
        sword.drawSwing(m_window, m_player.getCenter(), m_lastSwordFacing);
    }

    m_player.draw(m_window);
    drawHud();
}

void Game::drawGameOver()
{
    m_window.setView(m_uiView);
    centerText("GAME OVER", {640.f, 160.f}, 48, sf::Color(214, 78, 82));
    centerText("Time: " + formatTime(m_finalTime), {640.f, 242.f}, 25, sf::Color(235, 235, 220));
    centerText("Best: " + formatTime(m_recordManager.getBestTime()), {640.f, 282.f}, 25, sf::Color(235, 235, 220));
    centerText("Wave reached: " + std::to_string(m_waveManager.getWave()), {640.f, 322.f}, 24, sf::Color(205, 205, 200));

    if (m_newRecord)
        centerText("NEW RECORD", {640.f, 356.f}, 24, sf::Color(246, 221, 124));

    m_againButton.draw(m_window, m_font ? &*m_font : nullptr);
    m_menuButton.draw(m_window, m_font ? &*m_font : nullptr);
    m_exitButton.draw(m_window, m_font ? &*m_font : nullptr);
}

void Game::drawHud()
{
    m_window.setView(m_uiView);

    sf::RectangleShape panel({360.f, 142.f});
    panel.setPosition({18.f, 18.f});
    panel.setFillColor(sf::Color(20, 22, 30, 220));
    panel.setOutlineColor(sf::Color(92, 89, 106));
    panel.setOutlineThickness(2.f);
    m_window.draw(panel);

    const float healthRatio = static_cast<float>(m_player.getHealth()) / static_cast<float>(m_player.getMaxHealth());
    sf::RectangleShape healthBack({220.f, 18.f});
    healthBack.setPosition({128.f, 34.f});
    healthBack.setFillColor(sf::Color(60, 35, 42));
    m_window.draw(healthBack);

    sf::RectangleShape healthFill({220.f * healthRatio, 18.f});
    healthFill.setPosition({128.f, 34.f});
    healthFill.setFillColor(sf::Color(190, 54, 64));
    m_window.draw(healthFill);

    drawText("HP", {42.f, 28.f}, 20);
    drawText(std::to_string(m_player.getHealth()) + "/" + std::to_string(m_player.getMaxHealth()), {130.f, 56.f}, 16);
    drawText("Weapon: " + m_player.getWeaponName(), {42.f, 82.f}, 18);
    drawText("Wave: " + std::to_string(m_waveManager.getWave()), {42.f, 112.f}, 18);
    drawText("Enemies: " + std::to_string(m_enemies.size()), {174.f, 112.f}, 18);
    drawText("DMG L" + std::to_string(m_player.getWeaponDamageLevel()) + "  SPD L" + std::to_string(m_player.getAttackSpeedLevel()), {42.f, 140.f}, 16, sf::Color(246, 221, 124));

    if (m_player.getSpeedBoostLeft() > 0.f)
        drawText("Move boost " + std::to_string(static_cast<int>(m_player.getSpeedBoostLeft() + 1.f)) + "s", {42.f, 164.f}, 16, sf::Color(117, 255, 211));

    drawText("Time " + formatTime(m_survivalTime), {1020.f, 28.f}, 22, sf::Color(236, 221, 144));
    drawText("Best " + formatTime(m_recordManager.getBestTime()), {1020.f, 58.f}, 18, sf::Color(220, 220, 210));
    if (m_survivalTime >= 300.f)
        drawText("Armored skeletons", {1020.f, 88.f}, 18, sf::Color(117, 174, 205));

    if (m_waveManager.isWaiting())
        centerText("Next wave in " + std::to_string(std::max(0, static_cast<int>(m_waveManager.getPauseLeft() + 1.f))), {640.f, 84.f}, 24, sf::Color(236, 221, 144));
}

void Game::drawText(const std::string& text, sf::Vector2f position, unsigned int size, sf::Color color)
{
    if (!m_font)
        return;

    sf::Text drawable(*m_font, text, size);
    drawable.setFillColor(color);
    drawable.setPosition(position);
    m_window.draw(drawable);
}

void Game::centerText(const std::string& text, sf::Vector2f center, unsigned int size, sf::Color color)
{
    if (!m_font)
        return;

    sf::Text drawable(*m_font, text, size);
    drawable.setFillColor(color);
    const sf::FloatRect bounds = drawable.getLocalBounds();
    drawable.setPosition({center.x - bounds.size.x * 0.5f - bounds.position.x, center.y - bounds.size.y * 0.5f - bounds.position.y});
    m_window.draw(drawable);
}

void Game::resetGame()
{
    m_state = GameState::Playing;
    m_survivalTime = 0.f;
    m_finalTime = 0.f;
    m_healthPackTimer = 4.f;
    m_speedBoosterTimer = 7.f;
    m_swordFlashTimer = 0.f;
    m_newRecord = false;
    m_enemies.clear();
    m_projectiles.clear();
    m_healthPacks.clear();
    m_speedBoosters.clear();
    m_weaponBoosters.clear();
    m_waveManager.reset();
    m_player.reset({1280.f, 900.f});
    m_gameView.setCenter(m_player.getCenter());
}

void Game::tryAttack()
{
    if (!m_player.canAttack())
        return;

    if (m_player.getWeaponType() == WeaponType::Sword)
    {
        auto* sword = dynamic_cast<Sword*>(&m_player.getWeapon());
        if (!sword)
            return;

        m_lastSwordBox = sword->hitBox(m_player.getCenter(), m_player.getFacing());
        m_lastSwordFacing = m_player.getFacing();
        m_swordFlashTimer = 0.11f;

        for (auto& enemy : m_enemies)
        {
            if (intersects(m_lastSwordBox, enemy->getBounds()))
            {
                const int damage = static_cast<int>(static_cast<float>(sword->damage()) * m_player.getWeaponDamageMultiplier());
                enemy->takeDamage(damage);
            }
        }
    }
    else
    {
        auto* bow = dynamic_cast<Bow*>(&m_player.getWeapon());
        if (bow)
        {
            const int damage = static_cast<int>(static_cast<float>(bow->damage()) * m_player.getWeaponDamageMultiplier());
            m_projectiles.push_back(bow->shoot(m_player.getCenter(), m_player.getFacing(), damage));
        }
    }

    m_player.restartAttackCooldown();
}

void Game::checkCollisions()
{
    for (auto& enemy : m_enemies)
    {
        const float attackDistance = enemy->isMage() ? 38.f : 48.f;
        if (vectorLength(enemy->getCenter() - m_player.getCenter()) <= attackDistance && enemy->canHitPlayer())
        {
            m_player.takeDamage(enemy->getDamage());
            enemy->restartHitCooldown();
        }
    }

    for (auto& projectile : m_projectiles)
    {
        if (!projectile.isAlive())
            continue;

        if (projectile.isEnemyProjectile())
        {
            if (intersects(projectile.getBounds(), m_player.getBounds()))
            {
                m_player.takeDamage(projectile.getDamage());
                projectile.destroy();
            }
            continue;
        }

        for (auto& enemy : m_enemies)
        {
            if (!enemy->isDead() && intersects(projectile.getBounds(), enemy->getBounds()))
            {
                enemy->takeDamage(projectile.getDamage());
                projectile.destroy();
                break;
            }
        }
    }

    m_healthPacks.erase(std::remove_if(m_healthPacks.begin(), m_healthPacks.end(),
        [this](const HealthPack& pack)
        {
            if (!intersects(pack.getBounds(), m_player.getBounds()))
                return false;

            m_player.heal(pack.getHealAmount());
            return true;
        }), m_healthPacks.end());

    m_speedBoosters.erase(std::remove_if(m_speedBoosters.begin(), m_speedBoosters.end(),
        [this](const SpeedBooster& booster)
        {
            if (!intersects(booster.getBounds(), m_player.getBounds()))
                return false;

            m_player.applySpeedBoost(booster.getDuration(), booster.getMultiplier());
            return true;
        }), m_speedBoosters.end());

    m_weaponBoosters.erase(std::remove_if(m_weaponBoosters.begin(), m_weaponBoosters.end(),
        [this](const WeaponBooster& booster)
        {
            if (!intersects(booster.getBounds(), m_player.getBounds()))
                return false;

            if (booster.getType() == WeaponUpgradeType::Damage)
                m_player.upgradeWeaponDamage();
            else
                m_player.upgradeAttackSpeed();

            return true;
        }), m_weaponBoosters.end());
}

void Game::spawnHealthPack()
{
    const sf::Vector2f center = m_map.randomFreePosition(m_rng, m_player.getCenter());
    m_healthPacks.emplace_back(center - sf::Vector2f{13.f, 13.f});
}

void Game::spawnSpeedBooster()
{
    const sf::Vector2f center = m_map.randomFreePosition(m_rng, m_player.getCenter());
    m_speedBoosters.emplace_back(center - sf::Vector2f{13.f, 13.f});
}

void Game::spawnWeaponBooster()
{
    std::uniform_int_distribution<int> typeDist(0, 1);
    const WeaponUpgradeType type = typeDist(m_rng) == 0 ? WeaponUpgradeType::Damage : WeaponUpgradeType::AttackSpeed;
    const sf::Vector2f center = m_map.randomFreePosition(m_rng, m_player.getCenter());
    m_weaponBoosters.emplace_back(center - sf::Vector2f{14.f, 14.f}, type);
}

void Game::removeDeadObjects()
{
    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
        [](const std::unique_ptr<Enemy>& enemy)
        {
            return enemy->isDead();
        }), m_enemies.end());

    m_projectiles.erase(std::remove_if(m_projectiles.begin(), m_projectiles.end(),
        [](const Projectile& projectile)
        {
            return !projectile.isAlive();
        }), m_projectiles.end());
}

void Game::finishGame()
{
    m_finalTime = m_survivalTime;
    m_newRecord = m_recordManager.updateIfBest(m_finalTime);
    m_state = GameState::GameOver;
}

void Game::updateViews()
{
    m_gameView.setCenter(m_player.getCenter());
}

bool Game::loadFont()
{
    const std::filesystem::path fallbackFont = "C:/Windows/Fonts/consola.ttf";

    sf::Font font;
    if (font.openFromFile(fallbackFont))
    {
        m_font = std::move(font);
        return true;
    }

    return false;
}

bool Game::loadTextureAtlas()
{
    const std::filesystem::path atlasPath = std::filesystem::path(PROJECT_ROOT) / "assets" / "textures" / "dungeon_atlas.png";
    return m_textureAtlas.loadFromFile(atlasPath);
}
