#include "Ai.hpp"

#include "Game.hpp"

std::pair<Field*, Field*> Ai::chooseBestMove(std::vector<Field>& fields, const Turn& turn) {
    Field* bestFrom = nullptr;
    Field* bestTo = nullptr;
    int bestScore = -1;

    for (Field& from : fields) {
        if (from.getState() == turn.currentPlayer) {
            for (Field& to : fields) {
                Move move(from, to);
                if (move.isLegal()) {

                    move.moveScore += move.isAdjacent() ? 2 : 1;

                    for (Field& neighbor : fields) {
                        Move adjacentCheck(to, neighbor);

                        if (adjacentCheck.isAdjacent() &&
                            neighbor.getState() != FieldState::None &&
                            neighbor.getState() != turn.currentPlayer &&
                            neighbor.getState() != FieldState::NotAvailable)
                        {
                            move.moveScore += 2;
                        }
                    }

                    if (move.moveScore > bestScore) {
                        bestScore = move.moveScore;
                        bestFrom = &from;
                        bestTo = &to;
                    }
                }
            }
        }
    }
    return {bestFrom, bestTo};
}

void Ai::commitMove(Field* from, Field* to, std::vector<Field>& fields, Turn& turn) {
    if (!from || !to) return;

    to->setState(from->getState());

    const Field mFrom = *from;
    const Field mTo = *to;
    Move move(mFrom, mTo);

    if (move.isJump())
        from->setState(FieldState::None);

    for (Field& neighbor : fields) {
        Move adjacent(move.toField, neighbor);

        if (adjacent.isAdjacent() &&
            neighbor.getState() != FieldState::None &&
            neighbor.getState() != FieldState::NotAvailable &&
            neighbor.getState() != turn.currentPlayer)
        {
            neighbor.setState(turn.currentPlayer);
        }
    }
}


bool Ai::isAiTurn(const Turn &turn) {
    return turn.currentPlayer == FieldState::Player2;
}

