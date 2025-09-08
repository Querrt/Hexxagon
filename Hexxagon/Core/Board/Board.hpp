#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Field.hpp"
#include "../TurnAndMoves/Turn.hpp"

class Board {
public:
    Board(std::vector<Field>& fields);
    void draw(sf::RenderWindow& window);
    const std::vector<Field>& getFields() const { return fields; }
    bool click(sf::RenderWindow& window, sf::Vector2i mousePos, Turn& turn);
    sf::Vector2f getFieldPosition(int q, int r, const sf::RenderWindow& window) const;
    void resetBoard();

    static bool displayIllegalMoveText;
    static bool displayIllegalMoveTextNotAField;
    static bool displayIllegalMoveTextFieldTaken;
    static bool displayIllegalMoveTextTooLongDistance;

private:
    std::vector<Field>& fields;
    const float cellSize = 20.0f;  // Rozmiar pojedynczego pola
};

// wskaźnik a nie referencja, bo referencja nie moze byc nullptr(musi sie do czegos odnosic)
static Field* selectedPawn = nullptr;
