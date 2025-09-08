#pragma once
#include <vector>

#include "Move.hpp"
#include "Turn.hpp"

class Ai {
public:
    static std::pair<Field*, Field*> chooseBestMove(std::vector<Field>& fields, const Turn& turn);
    static void commitMove(Field* from, Field* to, std::vector<Field>& fields, Turn& turn);
    static bool isAiTurn(const Turn& turn);
};