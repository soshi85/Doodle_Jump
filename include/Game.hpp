#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <optional>
#include <vector>

#include "ResourceManager.hpp"
#include "Player.hpp"
#include "PlatformManager.hpp"
#include "HighScoreManager.hpp"
#include "Button.hpp"
#include "Monster.hpp"
#include "Bullet.hpp"
#include "GameSetting.hpp"

enum class GameState{
    MainMenu,
    Gameplay,
    GameOver, 
    Settings
};

class Game{

private:
    sf::RenderWindow window;
    GameState currentState;

    ResourceManager<sf::Texture> textureManager;
    ResourceManager<sf::Font> fontManager;
    HighScoreManager highScoreManager;

    std::unique_ptr<Player> player;
    PlatformManager platformManager;

    float shootTimer = 0.f; 

    sf::Sprite backgroundSprite;

    sf::Text titleText;
    sf::Text menuHighScoreText;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text finalScoreText;

    GameSettings gameSettings;

    sf::Text settingsButtonText;
    sf::Text difficultyText;
    sf::Text volumeText;
    sf::Text backText;

    sf::RectangleShape volumeBarBg;  
    sf::RectangleShape volumeBarFill;

    Button startButton;
    Button restartButton;
    Button menuButton;

    std::vector<Bullet*> bullets;

    ResourceManager<sf::SoundBuffer> soundManager;
    sf::Sound jumpSound;
    sf::Sound shootSound;
    sf::Music bgMusic;

    
    void updateAudioVolume();
    void updateSettingsTexts();
    void renderSettings();
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    void updateGameplay(sf::Time deltaTime);
    void renderMenu();
    void renderGameplay();
    void renderGameOver();

    void handleMouseClick(sf::Vector2f worldPos);
    void startNewGame();

public:
    Game();
    void run();
};
