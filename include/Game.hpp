#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "Player.hpp"
#include <memory>
#include <vector>
#include "Platform.hpp"
#include "NormalPlatform.hpp"




enum class GameState{
    MainMenu,
    Gameplay,
    GameOver
};

class Game {
private:
    sf::RenderWindow window;
    GameState currentState;
    ResourceManager<sf::Texture> textureManager;
    std::unique_ptr<Player> player;
    sf::Sprite backgroundSprite;

    std::vector<std::unique_ptr<Platform>> platforms;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();

public:
    Game();
    void run(); 
};