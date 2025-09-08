#include "Board.hpp"

#include <valarray>

#include "BoardCreator.hpp"  // Dodajemy nagłówek z funkcją createBoard
#include "../TurnAndMoves/Game.hpp"
#include "../TurnAndMoves/Move.hpp"
#include "../TurnAndMoves/Turn.hpp"
#include "fmt/printf.h"

Board::Board(std::vector<Field>& fields) : fields(fields) {}

auto Board::draw(sf::RenderWindow& window) -> void {
    const Field* selectedField = nullptr;

    for (const auto& field : fields) {
        if (field.selected) {
            selectedField = &field;
            break;
        }
    }

    for (const auto& field : fields) {
        sf::CircleShape cell(cellSize, 6);

        int q = field.getQ();
        int r = field.getR();

        // Współrzędne dla układu heksagonalnego
        float x = ((q+r/2.0)*cellSize)*2 + window.getSize().x/2.0;
        float y = (r*cellSize*std::sqrt(3)/2.0)*2 + window.getSize().y/2.0;


        // Ustawienie pozycji
        cell.setPosition({x, y});

        // Kolorowanie w zależności od stanu pola
        switch (field.getState()) {
            case FieldState::None:
                cell.setFillColor(sf::Color::White);
                break;
            case FieldState::Player1:
                cell.setFillColor(sf::Color::Blue);
                break;
            case FieldState::Player2:
                cell.setFillColor(sf::Color::Red);
                break;
            case FieldState::NotAvailable:
                cell.setFillColor(sf::Color::Transparent);
            break;
        }

        cell.setOutlineThickness(0);

        if (field.selected) {
            cell.setOutlineThickness(10);
            cell.setOutlineColor(sf::Color(0, 255, 0));
        }
        else if (selectedField) {
            Move move(*selectedField, field);
            if (move.isLegal()) {
                cell.setOutlineThickness(5);
                if (move.isJump()) {
                    cell.setOutlineColor(sf::Color(255, 216, 0));
                } else if (move.isAdjacent()) {
                    cell.setOutlineColor(sf::Color(220, 255, 0));
                }
            }
        }
        // Rysowanie pola
        window.draw(cell);
    }
}

bool Board::click(sf::RenderWindow& window,const sf::Vector2i mousePos, Turn& turn) {
    auto out = false;

    for (auto& field : fields) {
        const int q = field.getQ();
        const int r = field.getR();

        const float x = (q+r/2.0) * cellSize*2 + window.getSize().x/2.0 + cellSize;
        const float y = r*cellSize*std::sqrt(3) + window.getSize().y/2.0 + cellSize/2;

        const float dx = mousePos.x - x;
        const float dy = mousePos.y - y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= cellSize) {
            if (selectedPawn == nullptr && field.getState() == turn.currentPlayer) {
                selectedPawn = &field;
                selectedPawn->selected = true;
            }
            else if (field.getState() == FieldState::None && selectedPawn != nullptr) {
                Move move(*selectedPawn, field);
                if (move.isLegal()) {
                    if (move.isAdjacent()) {
                        Game::claimAdjacent(field, turn, fields);
                        field.setState(selectedPawn->getState());
                        selectedPawn->selected = false;
                        selectedPawn = nullptr;
                    } else {
                        Game::claimAdjacent(field, turn, fields);
                        field.setState(selectedPawn->getState());
                        selectedPawn->setState(FieldState::None);
                        selectedPawn->selected = false;
                        selectedPawn = nullptr;
                    }
                    out = true;
                } else if (distance>2) {
                    displayIllegalMoveTextTooLongDistance = true;
                } else if (field.getState() == FieldState::Player1 || field.getState() == FieldState::Player2) {
                    displayIllegalMoveTextFieldTaken = true;
                } else {
                    displayIllegalMoveTextNotAField = true;
                }
            }
            else if (selectedPawn==&field) {
                selectedPawn->selected = false;
                selectedPawn = nullptr;
            } else if (field.getState() == turn.currentPlayer && selectedPawn !=nullptr) {
                selectedPawn->selected = false;
                selectedPawn = &field;
                selectedPawn->selected = true;
            }
        }
    }
    return out;
}

void Board::resetBoard() {
    auto newFields = createBoard();
    fields.clear();
    fields.insert(fields.end(), newFields.begin(), newFields.end());
    selectedPawn = nullptr;
}

bool Board::displayIllegalMoveText = false;
bool Board::displayIllegalMoveTextNotAField = false;
bool Board::displayIllegalMoveTextFieldTaken = false;
bool Board::displayIllegalMoveTextTooLongDistance = false;
