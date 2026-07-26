#include "Game.hpp"
#include "Constants.hpp"
#include "GameSetting.hpp" 
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
      
      gameSettings("settings.txt"),
      
      settingsButtonText(fontManager.get("fonts/ariblk.ttf")),
      difficultyText(fontManager.get("fonts/ariblk.ttf")),
      volumeText(fontManager.get("fonts/ariblk.ttf")),
      backText(fontManager.get("fonts/ariblk.ttf")), 
      
      startButton(fontManager.get("fonts/ariblk.ttf"), "Start", sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH)/2.f-70.f, 340.f}, sf::Vector2f{140.f, 46.f}, sf::Color{90, 170, 90}),
      restartButton(fontManager.get("fonts/ariblk.ttf"), "Restart", sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH) / 2.f - 70.f, 380.f}, sf::Vector2f{140.f, 46.f}, sf::Color{230, 160, 60}),
      menuButton(fontManager.get("fonts/ariblk.ttf"), "Menu", sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH) / 2.f - 70.f, 440.f}, sf::Vector2f{140.f, 46.f}, sf::Color{90, 140, 220}),
    
      jumpSound(soundManager.get("sounds/Jumping_Sound.wav")),
      shootSound(soundManager.get("sounds/Shooting_Sound.wav"))
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

    settingsButtonText.setString("Settings");
    settingsButtonText.setCharacterSize(30);
    settingsButtonText.setPosition(sf::Vector2f{centerX, 500.f}); 
    centerOrigin(settingsButtonText);

    volumeBarBg.setSize(sf::Vector2f{200.f, 20.f});
    volumeBarBg.setFillColor(sf::Color{150, 150, 150});
    volumeBarBg.setPosition(sf::Vector2f{centerX - 100.f, 390.f}); 
    
    volumeBarFill.setSize(sf::Vector2f{200.f, 20.f});
    volumeBarFill.setFillColor(sf::Color{50, 200, 50});
    volumeBarFill.setPosition(sf::Vector2f{centerX - 100.f, 390.f});

    backText.setString("Back to Menu");
    backText.setCharacterSize(30);
    backText.setPosition(sf::Vector2f{centerX, 500.f});
    centerOrigin(backText);

    if (bgMusic.openFromFile("sounds/MainMenu_Song.flac")) {
        bgMusic.setLooping(true);
        bgMusic.play();
    }
    updateAudioVolume();
    updateSettingsTexts();
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
        if (bgMusic.getStatus() != sf::SoundSource::Status::Playing) {
                bgMusic.play();
            }
        if (startButton.contains(worldPos)){
            startNewGame();
        } 
        else if (settingsButtonText.getGlobalBounds().contains(worldPos)) {
            currentState = GameState::Settings;
            if (bgMusic.getStatus() != sf::SoundSource::Status::Playing) {
                bgMusic.play(); 
            }
        }
    } 
    else if (currentState == GameState::Settings) {
        if (bgMusic.getStatus() != sf::SoundSource::Status::Playing) {
                bgMusic.play(); 
        }
        if (difficultyText.getGlobalBounds().contains(worldPos)) {
            Difficulty current = gameSettings.getDifficulty();
            if (current == Difficulty::Easy) gameSettings.setDifficulty(Difficulty::Medium);
            else if (current == Difficulty::Medium) gameSettings.setDifficulty(Difficulty::Hard);
            else gameSettings.setDifficulty(Difficulty::Easy);
            
            gameSettings.save();
            updateSettingsTexts();
        } 
        else if (volumeBarBg.getGlobalBounds().contains(worldPos) || volumeText.getGlobalBounds().contains(worldPos)) {
            
            float clickX = worldPos.x - volumeBarBg.getPosition().x;
            float newVol = (clickX / volumeBarBg.getSize().x) * 100.f;
            
            if (newVol < 0.f) newVol = 0.f;
            if (newVol > 100.f) newVol = 100.f;
            
            gameSettings.setVolume(newVol);
            gameSettings.save();
            updateSettingsTexts();
            updateAudioVolume(); 
        }
        else if (backText.getGlobalBounds().contains(worldPos)) {
            currentState = GameState::MainMenu;
        }
    }
    else if (currentState == GameState::GameOver){
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
    sf::Texture& texShoot = textureManager.get("assets/Shooting@Pose.png"); 
    sf::Texture& texSnout = textureManager.get("assets/Nose.png");

    player = std::make_unique<Player>(texLeft, texRight, texShoot, texSnout);
    player->jump();

    platformManager.setDifficulty(gameSettings.getDifficulty());

    platformManager.reset(sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH)/2.f,
                                        static_cast<float>(Constants::WINDOW_HEIGHT)-40.f});
    
    bgMusic.pause();
    
    currentState = GameState::Gameplay;
}

void Game::update(sf::Time deltaTime){
    if (currentState == GameState::Gameplay) {
        updateGameplay(deltaTime);
    }
}

void Game::updateGameplay(sf::Time deltaTime){

    shootTimer -= deltaTime.asSeconds();

    player->update(deltaTime);

    if (player->wantsToShoot() && shootTimer <= 0.f) {
        shootSound.play();
        
        sf::Texture& bulletTex = textureManager.get("assets/Pea.png"); 
        sf::Vector2f playerPos = player->getPosition();
        sf::FloatRect playerBounds = player->getBounds();
        
        sf::Vector2f bulletStart(playerPos.x + playerBounds.size.x / 2.0f, playerPos.y);
        bullets.push_back(new Bullet(bulletTex, bulletStart));

        shootTimer = (gameSettings.getDifficulty() == Difficulty::Easy) ? 0.2f : 0.5f; 
    }

    float scrollThreshold = static_cast<float>(Constants::WINDOW_HEIGHT) * Constants::SCROLL_THRESHOLD_RATIO;
    float scrollAmount = 0.f;
    
    if (player->getPosition().y < scrollThreshold){
        scrollAmount = scrollThreshold - player->getPosition().y;
        player->move({0.f, scrollAmount});
    }

    platformManager.update(deltaTime, scrollAmount);
    
    float prevVelY = player->getVelocityY();
    
    platformManager.checkCollisions(*player);

    if (prevVelY >= 0.f && player->getVelocityY() < 0.f) {
        jumpSound.play();
    }

    for (auto it = bullets.begin(); it != bullets.end(); ) {
        (*it)->update(deltaTime);
        if (scrollAmount > 0.f) {
            (*it)->move({0.f, scrollAmount});
        }
        
        if ((*it)->isOffScreen()) {
            delete *it;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

    auto& activeMonsters = platformManager.getMonsters();

    for (auto bIt = bullets.begin(); bIt != bullets.end(); ) {
        bool bulletHit = false;
        for (auto mIt = activeMonsters.begin(); mIt != activeMonsters.end(); ) {
            if ((*bIt)->getBounds().findIntersection((*mIt)->getBounds()).has_value()) {
                bulletHit = true;
                if ((*mIt)->takeDamage()) { 
                    mIt = activeMonsters.erase(mIt); 
                } else {
                    ++mIt;
                }
                break; 
            } else {
                ++mIt;
            }
        }
        
        if (bulletHit) {
            delete *bIt;
            bIt = bullets.erase(bIt);
        } else {
            ++bIt;
        }
    }

    sf::FloatRect playerBounds = player->getBounds();
    for (auto mIt = activeMonsters.begin(); mIt != activeMonsters.end(); ++mIt) {
        if (playerBounds.findIntersection((*mIt)->getBounds()).has_value()) {
            if (player->getVelocityY() > 0.f && (player->getPosition().y + playerBounds.size.y) < ((*mIt)->getPosition().y + 30.f)) {
                player->jump(Constants::SPRING_JUMP_VELOCITY); 
            } else {
                currentState = GameState::GameOver;
            }
        }
    }

    auto& activeHoles = platformManager.getHoles();
    for (auto& hole : activeHoles) {
        if (playerBounds.findIntersection(hole->getBounds()).has_value()) {
            player->startSucking(hole->getCenter()); 
        }
    }

    if (player->isFullySucked()) {
        currentState = GameState::GameOver;
    }

    scoreText.setString("Score: " + std::to_string(player->getScore()));

    if (player->getPosition().y > static_cast<float>(Constants::WINDOW_HEIGHT)) {
        currentState = GameState::GameOver;
    }

    if (currentState == GameState::GameOver) {
        Difficulty currentDiff = gameSettings.getDifficulty();
        highScoreManager.reportScore(player->getScore(), currentDiff);
        
        finalScoreText.setString("Score: " + std::to_string(player->getScore()) +  "   High Score: " + std::to_string(highScoreManager.get(currentDiff)));
        centerOrigin(finalScoreText);
    }
}

void Game::render(){
    window.clear(sf::Color{200, 240, 255});
    window.draw(backgroundSprite);

    if (currentState == GameState::MainMenu){
        renderMenu();
    } 
    else if (currentState == GameState::Settings) {
        renderSettings();
    } 
    else if (currentState == GameState::Gameplay){
        renderGameplay();
    } 
    else {
        renderGameOver();
    }

    window.display();
}

void Game::renderMenu(){
    menuHighScoreText.setString("High Score: " + std::to_string(highScoreManager.get(gameSettings.getDifficulty())));
    centerOrigin(menuHighScoreText);
    window.draw(titleText);
    window.draw(menuHighScoreText);
    
    startButton.render(window);
    
    window.draw(settingsButtonText);
}

void Game::renderGameplay() {
    platformManager.render(window);    
    for (auto b : bullets) b->render(window);
    if (player) player->render(window);
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

void Game::updateSettingsTexts() {
    std::string diffStr = "Medium";
    if (gameSettings.getDifficulty() == Difficulty::Easy) diffStr = "Easy";
    else if (gameSettings.getDifficulty() == Difficulty::Hard) diffStr = "Hard";
    
    difficultyText.setString("Difficulty: " + diffStr);
    difficultyText.setCharacterSize(30);
    difficultyText.setPosition(sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, 250.f});
    centerOrigin(difficultyText);
    
    volumeText.setString("Volume: " + std::to_string(static_cast<int>(gameSettings.getVolume())) + "%");
    volumeText.setCharacterSize(30);
    volumeText.setPosition(sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, 350.f});
    centerOrigin(volumeText);
    
    float vol = gameSettings.getVolume();
    volumeBarFill.setSize(sf::Vector2f{200.f * (vol / 100.f), 20.f});
}

void Game::renderSettings() {
    window.draw(titleText);
    window.draw(difficultyText);
    window.draw(volumeText);
    window.draw(volumeBarBg);
    window.draw(volumeBarFill);
    window.draw(backText);
}

void Game::updateAudioVolume() {
    float vol = gameSettings.getVolume();
    bgMusic.setVolume(vol * 0.4f); 
    jumpSound.setVolume(vol);
    shootSound.setVolume(vol);
}