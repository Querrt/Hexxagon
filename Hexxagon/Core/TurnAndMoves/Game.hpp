#pragma once

#include <vector>

#include "Turn.hpp"
#include "../../GUI/Score.hpp"
class Field;

class Game {
public:
    static void claimAdjacent(Field& field, const Turn& turn, std::vector<Field>& fields);
    static GameState determineWinner(const Score& score);
    static bool hasViableMove(const Turn& turn, const std::vector<Field>& fields);
    static bool hasPawn(const Score& score);
    static void endGame(const Score& score, const sf::Font& font, sf::RenderWindow &window);

};
