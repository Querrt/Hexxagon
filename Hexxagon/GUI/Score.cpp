#include "Score.hpp"

#include "../Core/Board/Field.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
Score::Score() : player1Score(3), player2Score(3), winner(GameState::InGame) {}

void Score::countPoints(const std::vector<Field> &fields) {
    auto p1Counter = 0;
    auto p2Counter = 0;

    for (const auto& field : fields) {
        if (field.getState()==FieldState::Player1)
            p1Counter++;
        else if (field.getState()==FieldState::Player2)
            p2Counter++;
    }
    player1Score = p1Counter;
    player2Score = p2Counter;
}

void Score::drawScoreBoard(sf::RenderWindow &window, const sf::Font& font, Turn& turn) {

    sf::RectangleShape p1scoreBar(sf::Vector2f(player1Score*3, 25));
    sf::RectangleShape p2scoreBar(sf::Vector2f(player2Score*3, 25));

    p1scoreBar.setFillColor(sf::Color::Blue);
    p1scoreBar.setPosition(
        {window.getSize().x - p1scoreBar.getSize().x - 10,
        window.getSize().y - p1scoreBar.getSize().y - 10});
    p2scoreBar.setFillColor(sf::Color::Red);
    p2scoreBar.setPosition(
        {window.getSize().x - p2scoreBar.getSize().x - 10,
            window.getSize().y - p1scoreBar.getSize().y - p2scoreBar.getSize().y - 10}
        );

    sf::Text p1ScoreNum(font, Field::toString(turn.currentPlayer));
    p1ScoreNum.setCharacterSize(25);
    p1ScoreNum.setFillColor(sf::Color::White);
    p1ScoreNum.setString(std::to_string(player1Score));
    p1ScoreNum.setPosition(
        {window.getSize().x - p1scoreBar.getSize().x - 15,
        window.getSize().y - p1scoreBar.getSize().y - 13}
        );

    sf::Text p2ScoreNum(font, Field::toString(turn.currentPlayer));
    p2ScoreNum.setCharacterSize(25);
    p2ScoreNum.setFillColor(sf::Color::White);
    p2ScoreNum.setString(std::to_string(player2Score));
    p2ScoreNum.setPosition(
        {window.getSize().x - p2scoreBar.getSize().x - 15,
            window.getSize().y - p1scoreBar.getSize().y - p2scoreBar.getSize().y - 13}
            );

    window.draw(p1scoreBar);
    window.draw(p2scoreBar);
    window.draw(p1ScoreNum);
    window.draw(p2ScoreNum);
}

void Score::drawPlayerTurnText(sf::RenderWindow &window, const sf::Font& font, Turn& turn) {
    sf::Text playerTurnText(
        font,
        Field::toString(turn.currentPlayer) + " turn " + std::to_string(turn.turnNumber)
        );
    playerTurnText.setCharacterSize(24);
    playerTurnText.setFillColor(
        turn.currentPlayer == FieldState::Player1
                ? sf::Color::Blue
                : sf::Color::Red
                );
    playerTurnText.setOutlineColor(sf::Color::White);
    playerTurnText.setOutlineThickness(2);

    playerTurnText.setPosition(
        {window.getSize().x - playerTurnText.getGlobalBounds().size.x - 10,
            10}
            );

    window.draw(playerTurnText);
}


