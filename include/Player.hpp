#pragma once
#include "Constants.hpp"
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Sprite sprite;
    sf::Sprite snoutSprite;
    sf::Vector2f velocity;
    const sf::Texture* textureShoot;
    float fireCooldown;
    bool isShootingState;
    float highestY;
    int score;
    const sf::Texture* textureLeft;
    const sf::Texture* textureRight;
    bool isSuckedStatus = false;
    sf::Vector2f blackHoleCenter;

public:

    Player(const sf::Texture& leftTex, const sf::Texture& rightTex);
    Player(const sf::Texture& leftTex, const sf::Texture& rightTex, const sf::Texture& shootTex, const sf::Texture& nose);

    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);
    void jump(float power = Constants::JUMP_VELOCITY);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    float getVelocityY() const;
    void move(sf::Vector2f offset);
    int getScore() const;
    bool wantsToShoot();
    void startSucking(sf::Vector2f centerPos);
    bool isFullySucked() const;
};

