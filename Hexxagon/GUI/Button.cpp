#include "Button.hpp"

Button::Button(const sf::Vector2f& size, const sf::Font& font, const std::string& inText, const std::optional<sf::Texture>& texture)
    : shape(size), text(font, inText), sprite() {
    shape.setFillColor(sf::Color::Transparent);

    auto shapeBounds = shape.getGlobalBounds();
    shape.setOrigin(shapeBounds.size / 2.f);

    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    auto textBounds = text.getGlobalBounds();
    text.setOrigin(textBounds.position + textBounds.size * 0.5f);

    if (texture) {
        sprite.emplace(*texture);
        sprite->setOrigin(sprite->getGlobalBounds().size / 2.f);
    }
}

void Button::setPosition(const sf::Vector2f& pos) {
    shape.setPosition(pos);
    text.setPosition(pos);
    if (sprite)
        sprite->setPosition(pos);
}
void Button::mouseHover(const sf::RenderWindow& window) {
    auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    if (shape.getGlobalBounds().contains(mousePos)) {
        text.setFillColor(sf::Color::Red);
    } else {
        text.setFillColor(sf::Color::White);
    }
}


void Button::draw(sf::RenderWindow& window) const {
    if (sprite)
        window.draw(*sprite);
    window.draw(shape);
    window.draw(text);
}

// Prawie sie poplakalem. Musisz ogarnac przed obrona
// chodzi o to ze od sfml3.0 eventy to jakies tam wskazniki blablalba a poza tym nie ma .type()
// przez te wskazniki event musi byc przekazywany jako optional
bool Button::isClicked(const sf::RenderWindow& window, const std::optional<sf::Event>& event) const {
    if (!event) return false;

    if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
        auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        return shape.getGlobalBounds().contains(mousePos);
    }
    return false;
}

void Button::setFontSize(const int& fontSize) {
    text.setCharacterSize(fontSize);
    auto textBounds = text.getGlobalBounds();
    text.setOrigin(textBounds.size * 0.5f);
}

void Button::setText(const std::string& newText) {
    text.setString(newText);
}


