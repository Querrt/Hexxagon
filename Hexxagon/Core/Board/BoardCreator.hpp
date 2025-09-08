#pragma once
#include <vector>

#include "Field.hpp"

auto createBoard() -> std::vector<Field>;
auto fillBoard(std::vector<FieldState>& board) -> void;