#pragma once

#include <SFML/Graphics.hpp>

class Button {
    sf::RectangleShape shape;
    sf::Text text;
    std::optional<sf::Sprite> sprite;

public:
    Button(
        const sf::Vector2f& size,
        const sf::Font& font,
        const std::string& inText,
        const std::optional<sf::Texture>& texture = std::nullopt
        );


    void setPosition(const sf::Vector2f& pos);
    void mouseHover(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
    bool isClicked(const sf::RenderWindow& window, const std::optional<sf::Event>& event) const;
    void setFontSize(const int& newFontSize);
    void setText(const std::string& newText);
};