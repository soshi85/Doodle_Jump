#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

#include "ResourceManager.hpp"
#include "Player.hpp"
#include "PlatformManager.hpp"
#include "HighScoreManager.hpp"
#include "Button.hpp"

enum class GameState{
    MainMenu,
    Gameplay,
    GameOver
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

    sf::Sprite backgroundSprite;

    sf::Text titleText;
    sf::Text menuHighScoreText;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text finalScoreText;

    Button startButton;
    Button restartButton;
    Button menuButton;

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
