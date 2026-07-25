#pragma once
#include "Constants.hpp"
#include <SFML/Graphics.hpp>

enum class MonsterType {
    Blue,  
    Green 
};

class Monster {
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    int health;
    MonsterType type; 

public:
    Monster(const sf::Texture& texture, sf::Vector2f startPos, int startHealth, MonsterType type);

    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    void move(sf::Vector2f offset);
    
    bool takeDamage();
    int getHealth() const;
    bool isOffScreen(float bottomY) const;
    sf::Vector2f getPosition() const;
};