#pragma once
#include <string>

enum struct FieldState {
    None,
    Player1,
    Player2,
    NotAvailable
};

class Field {
public:
    Field(int q, int r, FieldState state = FieldState::None);

    int getQ() const;
    int getR() const;
    FieldState getState() const;
    void setState(FieldState state);
    bool isAvailable() const;
    bool selected;

    static std::string toString(FieldState state);

private:
    int q;
    int r;
    FieldState state;
};