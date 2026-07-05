#pragma once

#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float highestY; 
    int score;
    
    const sf::Texture* textureLeft;
    const sf::Texture* textureRight;

public:

    Player(const sf::Texture& leftTex, const sf::Texture& rightTex);
    
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);
    void jump(float power = 600.0f);
    
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    float getVelocityY() const;
    void move(sf::Vector2f offset);
    int getScore() const;
};

