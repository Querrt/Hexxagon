#pragma once

#include "../Core/TurnAndMoves/Turn.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

enum class GameState {
    Player1,
    Player2,
    InGame,
    Tie
};

class Score {
public:
    Score();

    void countPoints(const std::vector<Field>& fields);

    void drawScoreBoard(sf::RenderWindow &window, const sf::Font& font, Turn& turn);
    void drawPlayerTurnText(sf::RenderWindow &window, const sf::Font& font, Turn& turn);

    int player1Score, player2Score;
    GameState winner;
};

