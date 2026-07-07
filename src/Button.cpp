#include "Button.hpp"

Button::Button(const sf::Font& font, const std::string& label, sf::Vector2f position, sf::Vector2f size, sf::Color color)
    : text(font, label, 20) {
    
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(color);
    
    shape.setOutlineColor(sf::Color{30, 30, 30});
    shape.setOutlineThickness(2.f);

    text.setFillColor(sf::Color::White);
    
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f});
    text.setPosition({position.x + size.x / 2.0f, position.y + size.y / 2.0f});
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::contains(sf::Vector2f point) const {
    return shape.getGlobalBounds().contains(point);
}