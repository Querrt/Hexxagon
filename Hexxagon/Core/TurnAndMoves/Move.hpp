#pragma once
#include <map>

#include "../Board/Field.hpp"

class Move {
public:
    Move(const Field& fromField, const Field& toField);
    Field fromField;
    Field toField;
    int moveScore;
    auto isLegal() const -> bool;
    auto isAdjacent() const -> bool;
    auto isJump() const -> bool;
    auto calculateDistance() const -> int;
};
