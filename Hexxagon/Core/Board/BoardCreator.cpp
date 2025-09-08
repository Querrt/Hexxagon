#include "BoardCreator.hpp"

#include <cmath>

auto createBoard() -> std::vector<Field> {
    auto board = std::vector<Field>();
    auto const radius = 4;

    // |q| <= radius && |r| <= radius && | -q-r | <= radius
    for (int q = -radius; q <= radius; ++q) {
        for (int r = -radius; r <= radius; ++r) {
            if (std::abs(q + r) <= radius) {
                if ((q==0 && r==-1) || (q==1 && r==0) || (q==-1 && r==1))
                    board.emplace_back(q, r, FieldState::NotAvailable);
                else if ((q==0 && r==-4) || (q==4 && r==0) || (q==-4 && r==4))
                    board.emplace_back(q, r, FieldState::Player1);
                else if ((q==-4 && r==0) || (q==4 && r==-4) || (q==0 && r==4))
                    board.emplace_back(q, r, FieldState::Player2);
                else
                    board.emplace_back(q, r, FieldState::None);
            }
        }
    }

    return board;
}
auto fillBoard(std::vector<FieldState>& board) -> void {}