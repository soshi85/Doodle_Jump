#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button{
private:
    sf::RectangleShape shape;
    sf::Text text;

public:
    Button(const sf::Font& font, const std::string& label,
           sf::Vector2f position, sf::Vector2f size, sf::Color color);

    void render(sf::RenderWindow& window);
    bool contains(sf::Vector2f point) const;
};
