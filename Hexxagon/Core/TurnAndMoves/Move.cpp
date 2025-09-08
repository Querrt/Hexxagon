#include "Move.hpp"

#include <vector>

#include "../Board/Field.hpp"
#include "fmt/printf.h"
#include "SFML/System/Vector2.hpp"

namespace sf {
    class RenderWindow;
}

Move::Move(const Field& fromField, const Field& toField) :
    fromField(fromField), toField(toField), moveScore(0)
{}

auto Move::calculateDistance() const -> int {
    int dq = std::abs(fromField.getQ() - toField.getQ());
    int dr = std::abs(fromField.getR() - toField.getR());
    int ds = std::abs((-fromField.getQ() - fromField.getR()) - (-toField.getQ() - toField.getR()));

    return std::max({dq, dr, ds});
}

auto Move::isLegal() const -> bool {
    int distance = calculateDistance();
    return (distance == 1 || distance == 2) && toField.getState()==FieldState::None;
}

auto Move::isAdjacent() const -> bool {
    return calculateDistance() == 1;
}

auto Move::isJump() const -> bool {
    return calculateDistance() == 2;
}

