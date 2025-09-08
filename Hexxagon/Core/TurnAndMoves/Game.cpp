#include "Game.hpp"

#include <vector>

#include "Move.hpp"
#include "fmt/printf.h"
#include "SFML/Graphics/Text.hpp"

void Game::claimAdjacent(Field& field, const Turn& turn, std::vector<Field>& fields) {
    for (Field& adjField : fields) {
        Move move(field, adjField);
        if (move.isAdjacent()) {
            if (adjField.getState()==FieldState::Player2 && turn.currentPlayer==FieldState::Player1)
                adjField.setState(FieldState::Player1);
            else if (adjField.getState()==FieldState::Player1 && turn.currentPlayer==FieldState::Player2)
                adjField.setState(FieldState::Player2);
        }
    }
}

GameState Game::determineWinner(const Score& score) {
    if (score.player1Score > score.player2Score)
        return GameState::Player1;
    if (score.player1Score < score.player2Score)
        return GameState::Player2;
    return GameState::Tie;
}

bool Game::hasViableMove(const Turn &turn, const std::vector<Field> &fields) {
    for (const Field& field : fields)
        if (field.getState()==turn.currentPlayer)
            for (const Field& moveTo : fields) {
                Move move (field, moveTo);
                if (move.isLegal()) {
                    return true;
                }
            }
    return false;
}

bool Game::hasPawn(const Score &score) {
    return score.player1Score > 0 && score.player2Score > 0;
}

void Game::endGame(const Score& score, const sf::Font& font, sf::RenderWindow &window) {
    auto winnerTextStr = std::string();
    switch(score.winner) {
        case GameState::Player1: winnerTextStr = "Player1\n  wins";
        break;
        case GameState::Player2: winnerTextStr = "Player2\n  wins";
        break;
        case GameState::Tie: winnerTextStr = "Tie";
        break;
        default: winnerTextStr = "WHOT";
    }
    sf::Text winnerText(font, winnerTextStr);
    winnerText.setCharacterSize(140);
    if (score.winner == GameState::Tie)
        winnerText.setFillColor(sf::Color::White);
    if (score.winner == GameState::Player1)
        winnerText.setFillColor(sf::Color::Blue);
    if (score.winner == GameState::Player2)
        winnerText.setFillColor(sf::Color::Red);
    winnerText.setOutlineThickness(20);
    winnerText.setOutlineColor(sf::Color::Black);

    winnerText.setPosition(
        {window.getSize().x/2 - winnerText.getGlobalBounds().size.x/2,
            window.getSize().y/2 - winnerText.getGlobalBounds().size.y/2}
            );

    window.draw(winnerText);
}



