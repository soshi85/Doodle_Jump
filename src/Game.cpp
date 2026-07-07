#include "Game.hpp"
#include "Constants.hpp"
#include <optional>
#include <string>

namespace {
    void centerOrigin(sf::Text& text) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
    }
}

Game::Game() : 

    window(sf::VideoMode(sf::Vector2u{Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), "Doodle Jump"),
      currentState(GameState::MainMenu),
      highScoreManager("highscore.txt"),
      platformManager(textureManager),
      backgroundSprite(textureManager.get("assets/background.png")), 
      
    
      titleText(fontManager.get("fonts/ariblk.ttf"), "DOODLE JUMP", 36),
      menuHighScoreText(fontManager.get("fonts/ariblk.ttf"), "High Score: 0", 18),
      scoreText(fontManager.get("fonts/ariblk.ttf"), "Score: 0", 20),
      gameOverText(fontManager.get("fonts/ariblk.ttf"), "YOU LOST", 40),
      finalScoreText(fontManager.get("fonts/ariblk.ttf"), "Score: 0", 18),
      
    
      startButton(fontManager.get("fonts/ariblk.ttf"), "Start", sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH)/2.f-70.f, 340.f}, sf::Vector2f{140.f, 46.f}, sf::Color{90, 170, 90}),
      restartButton(fontManager.get("fonts/ariblk.ttf"), "Restart", sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH) / 2.f - 70.f, 380.f}, sf::Vector2f{140.f, 46.f}, sf::Color{230, 160, 60}),
      menuButton(fontManager.get("fonts/ariblk.ttf"), "Menu", sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH) / 2.f - 70.f, 440.f}, sf::Vector2f{140.f, 46.f}, sf::Color{90, 140, 220})
{
    window.setFramerateLimit(60);
    float centerX = static_cast<float>(Constants::WINDOW_WIDTH) / 2.f;
    
    centerOrigin(titleText);
    titleText.setPosition({centerX, 180.f});
    
    centerOrigin(menuHighScoreText);
    menuHighScoreText.setPosition({centerX, 240.f});
    
    scoreText.setPosition({10.f, 10.f});
    
    centerOrigin(gameOverText);
    gameOverText.setPosition({centerX, 200.f});
    
    centerOrigin(finalScoreText);
    finalScoreText.setPosition({centerX, 260.f});
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()){
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents(){

    while (const std::optional<sf::Event> event = window.pollEvent()){
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } 

        else if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {

            sf::Vector2f worldPos = window.mapPixelToCoords(mouseBtn->position);
             handleMouseClick(worldPos);
        }
    }
}

void Game::handleMouseClick(sf::Vector2f worldPos){
    if (currentState == GameState::MainMenu){
        if (startButton.contains(worldPos)){
            startNewGame();
        }
    } else if (currentState == GameState::GameOver){
        if (restartButton.contains(worldPos)){
            startNewGame();
        } else if (menuButton.contains(worldPos)){
            currentState = GameState::MainMenu;
        }
    }
}

void Game::startNewGame(){
    sf::Texture& texLeft = textureManager.get("assets/left_doodle.png");
    sf::Texture& texRight = textureManager.get("assets/right_doodle.png");

    player = std::make_unique<Player>(texLeft, texRight);
    player->jump();

    platformManager.reset(sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH)/2.f,
                                        static_cast<float>(Constants::WINDOW_HEIGHT)-40.f});

    currentState = GameState::Gameplay;
}

void Game::update(sf::Time deltaTime){
    if (currentState == GameState::Gameplay) {
        updateGameplay(deltaTime);
    }
}

void Game::updateGameplay(sf::Time deltaTime){
    player->update(deltaTime);
    float scrollThreshold = static_cast<float>(Constants::WINDOW_HEIGHT) * Constants::SCROLL_THRESHOLD_RATIO;
    float scrollAmount = 0.f;
    
    if (player->getPosition().y < scrollThreshold){
        scrollAmount = scrollThreshold - player->getPosition().y;
        player->move({0.f, scrollAmount});
    }

    platformManager.update(deltaTime, scrollAmount);
    platformManager.checkCollisions(*player);

    scoreText.setString("Score: " + std::to_string(player->getScore()));

    if (player->getPosition().y > static_cast<float>(Constants::WINDOW_HEIGHT)) {
        highScoreManager.reportScore(player->getScore());
        finalScoreText.setString("Score: " + std::to_string(player->getScore()) +  "   High Score: " + std::to_string(highScoreManager.get()));
        centerOrigin(finalScoreText);
        currentState = GameState::GameOver;
    }
}

void Game::render(){

    window.clear(sf::Color{200, 240, 255});
    window.draw(backgroundSprite);

    if (currentState == GameState::MainMenu){
        renderMenu();
    } else if (currentState == GameState::Gameplay){
        renderGameplay();
    } else {
        renderGameOver();
    }

    window.display();
}

void Game::renderMenu(){
    menuHighScoreText.setString("High Score: " + std::to_string(highScoreManager.get()));
    centerOrigin(menuHighScoreText);

    window.draw(titleText);
    window.draw(menuHighScoreText);
    startButton.render(window);
}

void Game::renderGameplay() {
    platformManager.render(window);
    if (player) {
        player->render(window);
    }

    window.draw(scoreText);
}

void Game::renderGameOver() {
    renderGameplay();

    sf::RectangleShape overlay(sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH), static_cast<float>(Constants::WINDOW_HEIGHT)});
    overlay.setFillColor(sf::Color{255, 255, 255, 180});
    window.draw(overlay);
    window.draw(gameOverText);
    window.draw(finalScoreText);
    restartButton.render(window);
    menuButton.render(window);
}