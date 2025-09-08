#pragma once
#include "../Board/Field.hpp"

class Turn {
public:
    Turn(FieldState currentPlayer=FieldState::Player1);

    int turnNumber;
    FieldState currentPlayer;
};

