#include "Field.hpp"

#include "fmt/color.h"

Field::Field(int q, int r, FieldState state)
    : selected(false), q(q), r(r), state(state) {}

int Field::getQ() const {
    return q;
}
int Field::getR() const {
    return r;
}
FieldState Field::getState() const {
    return state;
}

void Field::setState(FieldState state) {
    this->state = state;
}
bool Field::isAvailable() const {
    return (state != FieldState::NotAvailable) && (state == FieldState::None);
}

std::string Field::toString(FieldState state) {
    switch(state) {
        case FieldState::None: return "None";
        case FieldState::Player1: return "Player1";
        case FieldState::Player2: return "Player2";
        case FieldState::NotAvailable: return "NotAvailable";
        default: return "Unknown";
    }
}

