#include "Game.hpp"
#include <optional> 


Game::Game() 
    : window(sf::VideoMode({400, 600}), "Doodle Jump"), 
      currentState(GameState::Gameplay),
      backgroundSprite(textureManager.get("assets/background.png")) 
{
    window.setFramerateLimit(60);
    
    sf::Texture& texLeft = textureManager.get("assets/left_doodle.png");
    sf::Texture& texRight = textureManager.get("assets/right_doodle.png");
    
    player = std::make_unique<Player>(texLeft, texRight);

    sf::Texture& normalPlatTex = textureManager.get("assets/normal_platform.png");
    platforms.push_back(std::make_unique<NormalPlatform>(normalPlatTex, sf::Vector2f{200.0f, 580.0f}));
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        processEvents();
        update(deltaTime); 
        render();
    }
}

void Game::processEvents() {

    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Game::update(sf::Time deltaTime) {
    if (currentState == GameState::Gameplay) {
        player->update(deltaTime);
        for (auto& platform : platforms) {
        
            platform->update(deltaTime);

        
            if (player->getVelocityY() > 0.0f) {
                sf::FloatRect playerBounds = player->getBounds();
                sf::FloatRect platBounds = platform->getBounds();

        
                if (playerBounds.findIntersection(platBounds).has_value()) {
        
                    float playerBottom = player->getPosition().y + (playerBounds.size.y / 2.0f);
                    float platTop = platform->getPosition().y - (platBounds.size.y / 2.0f);

        
                    if (playerBottom < platTop + 15.0f) {
                        player->jump(); 
                    }
                }
            }
        }
    }
}

void Game::render() {
    
    window.clear(sf::Color(200, 240, 255));
    
    window.draw(backgroundSprite);
    

    if (currentState == GameState::Gameplay) {
        for (auto& platform : platforms) {
            platform->render(window);
        }
        
        if (player) {
            player->render(window);
        }
    }
    
    window.display();
}